#pragma once
#include <cstddef>                      // size_t
#include <iomanip>
#include <iostream>
#include <iterator>                     // begin, end
#include <boost/assert.hpp>
#include <dctl/walk/transposition.hpp>
#include <dctl/hash/dual_map.hpp>
#include <dctl/hash/signature_extractor.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/successor/count.hpp>
#include <dctl/successor/generate.hpp>
#include <dctl/utility/int.hpp>
#include <dctl/utility/statistics.hpp>
#include <dctl/utility/timer.hpp>

#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/notation/string.hpp>

namespace dctl {
namespace walk {

class Root
{
public:
        template<typename Position>
        NodeCount perft(Position const& p, int depth)
        {
                NodeCount leafs = 0;

                Timer timer;
                announce(p, depth);
                for (auto i = 1; i <= depth; ++i) {
                        statistics_.reset();
                        leafs = driver(p, i, 0);
                        timer.split();
                        report(i, leafs, timer);
                }
                summary();
                return leafs;
        }

        template<typename Position>
        NodeCount divide(Position const& p, int depth)
        {
                NodeCount leafs = 0;
                NodeCount move_leafs;

                Timer timer;
                auto const moves = successor::generate(p);

                announce(p, depth, moves.size());
                for (auto i = 0; i < static_cast<int>(moves.size()); ++i) {
                        statistics_.reset();
                        print_move(notation::write(p, moves[i]), i);

                        auto q = p;
                        q.attach(p);
                        q.make(moves[i]);
                        move_leafs = driver(q, depth - 1, 0);
                        leafs += move_leafs;

                        timer.split();
                        report(depth - 1, move_leafs, timer);
                }
                summary(leafs);
                return leafs;
        }

        template<typename Position>
        NodeCount test(Position const& p, int depth)
        {
                return driver(p, depth, 0);
        }

        void resize_hash(std::size_t s)
        {
                TT.resize(s);
        }

        void clear_hash()
        {
                TT.clear();
        }

private:
        template<typename Position>
        void announce(Position const& p, int depth)
        {
                std::cout << setup::diagram<pdn::protocol>()(p);
                std::cout << setup::write<pdn::protocol>()(p) << "\n";
                std::cout << "Searching to nominal depth=" << depth << "\n\n";
        }

        template<typename Position>
        void announce(Position const& p, int depth, int num_moves)
        {
                announce(p, depth);
                std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << depth - 1 << "\n";
                std::cout << "\n";
        }

        void report(int depth, NodeCount leafs, Timer const& timer)
        {
                std::cout << "info";

                std::cout << " depth ";
                std::cout << std::setw( 2) << depth;

                std::cout << " leafs ";
                std::cout << std::setw(12) << std::right << leafs;

                std::cout << " nodes ";
                std::cout << std::setw(12) << std::right << statistics_.nodes();

                std::cout << " time ";
                std::cout << std::setw( 6) << timer.lap();

                double const nps = static_cast<double>(1000 * statistics_.nodes()) / static_cast<double>(timer.lap());
                std::cout << " nps ";
                std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
                std::cout << std::setw( 7) << nps;

                double const hashfull = 1000 * (static_cast<double>(TT.size()) / static_cast<double>(TT.capacity()));
                std::cout << " hashfull ";
                std::cout << std::setw( 4) << std::right << hashfull;

                std::cout << std::endl;
        }

        void summary() const
        {
                std::cout << std::endl;
        }

        void summary(NodeCount leafs) const
        {
                std::cout << "Total leafs: " << leafs << "\n\n";
        }

        void print_move(std::string const& move, int i)
        {
                std::cout << std::setw(2) << (i + 1) << "." << move << " ";
        }

        template<typename Position>
        NodeCount driver(Position const& p, int depth, int ply)
        {
                return (depth == 0)? leaf(p, depth, ply) : hash(p, depth, ply);
        }

        template<typename Position>
        NodeCount leaf(Position const& p, int depth, int ply)
        {
                statistics_.update(ply);

                if (depth == 0)
                        return 1;

                auto const moves = successor::generate(p);
                NodeCount leafs = 0;
                for (auto const& m: moves) {
                        auto q = p;
                        q.attach(p);
                        q.make(m);
                        leafs += leaf(q, depth - 1, ply + 1);
                }
                return leafs;
        }

        template<typename Position>
        NodeCount bulk(Position const& p, int depth, int ply)
        {
                statistics_.update(ply);

                auto const moves = successor::generate(p);
                if (depth == 1)
                        return moves.size();

                NodeCount leafs = 0;
                for (auto const& m: moves) {
                        auto q = p;
                        q.attach(p);
                        q.make(m);
                        leafs += bulk(q, depth - 1, ply + 1);
                }
                return leafs;
        }

        template<typename Position>
        NodeCount count(Position const& p, int depth, int ply)
        {
                statistics_.update(ply);

                if (depth == 1)
                        return successor::count(p);

                auto const moves = successor::generate(p);
                NodeCount leafs = 0;
                for (auto const& m: moves) {
                        auto q = p;
                        q.attach(p);
                        q.make(m);
                        leafs += count(q, depth - 1, ply + 1);
                }
                return leafs;
        }

        template<typename Position>
        NodeCount hash(Position const& p, int depth, int ply)
        {
                statistics_.update(ply);

                auto const TT_entry = TT.find(p);
                if (TT_entry && TT_entry->depth() == depth)
                        return TT_entry->leafs();

                NodeCount leafs;
                if (depth == 1) {
                        leafs = successor::count(p);
                } else {
                        auto const moves = successor::generate(p);
                        leafs = 0;
                        for (auto const& m: moves) {
                                auto q = p;
                                q.attach(p);
                                q.make(m);
                                leafs += hash(q, depth - 1, ply + 1);
                        }
                }

                TT.insert(p, { leafs, depth } );
                return leafs;
        }

        // 32-byte hash entries: 24-byte piece lists signature, 8-byte (59-bit leafs, 5-bit depth) content
        // 8-way buckets on 64-byte cache lines, (2^25 entries = 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::DualMap<hash::MaterialExtractor, Transposition> TranspositionTable;
        TranspositionTable TT;

        // representation
        Statistics statistics_;
};

}       // namespace walk
}       // namespace dctl
