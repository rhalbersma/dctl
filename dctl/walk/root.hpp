#pragma once
#include <algorithm>                    // accumulate
#include <cstddef>                      // size_t
#include <iomanip>
#include <iostream>
#include <iterator>                     // distance
#include <boost/assert.hpp>             // BOOST_ASSERT
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

template<typename Position>
class Root
{
public:
        NodeCount perft(Position const& p, int max_depth)
        {
                BOOST_ASSERT(1 <= max_depth);
                NodeCount leaf_nodes = 0;
                Timer timer;
                announce(p, max_depth);
                for (auto depth = 1; depth <= max_depth; ++depth) {
                        statistics_.reset();
                        leaf_nodes = driver(p, depth, 0);
                        timer.split();
                        report(depth, leaf_nodes, timer);
                }
                summary();
                return leaf_nodes;
        }

        NodeCount divide(Position const& p, int depth)
        {
                NodeCount leaf_nodes = 0;
                NodeCount sub_count;

                Timer timer;
                Arena<Move> a;
                auto const moves = successor::generate(p, a);

                announce(p, depth, moves.size());
                for (auto const& m: moves) {
                        statistics_.reset();
                        auto const i = std::distance(&m, &moves[0]);
                        print_move(notation::write(p, moves[i]), i);
                        sub_count = driver(successor::make_copy(p, moves[i]), depth - 1, 1);
                        leaf_nodes += sub_count;

                        timer.split();
                        report(depth - 1, sub_count, timer);
                }
                summary(leaf_nodes);
                return leaf_nodes;
        }

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
        void announce(Position const& p, int depth)
        {
                std::cout << setup::diagram<pdn::protocol>()(p);
                std::cout << setup::write<pdn::protocol>()(p) << "\n";
                std::cout << "Searching to nominal depth=" << depth << "\n\n";
        }

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
                std::cout << std::setw( 6) << timer.lap().count();

                double const nps = static_cast<double>(1000 * statistics_.nodes()) / static_cast<double>(timer.lap().count());
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

        NodeCount driver(Position const& p, int depth, int ply)
        {
                return (depth == 0)? perft_impl(p, depth, ply) : perft_hash(p, depth, ply);
        }

        NodeCount perft_impl(Position const& p, int depth, int ply)
        {
                BOOST_ASSERT(0 <= depth);
                statistics_.update(ply);

                if (depth == 0)
                        return 1;

                Arena<Move> a;
                auto const moves = successor::generate(p, a);
                NodeCount leaf_nodes = 0;
                for (auto const& m: moves)
                        leaf_nodes += perft_impl(successor::make_copy(p, m), depth - 1, ply + 1);
                return leaf_nodes;
        }

        NodeCount perft_bulk(Position const& p, int depth, int ply)
        {
                BOOST_ASSERT(0 < depth);
                statistics_.update(ply);

                Arena<Move> a;
                auto const moves = successor::generate(p, a);
                if (depth == 1)
                        return moves.size();

                NodeCount leaf_nodes = 0;
                for (auto const& m: moves)
                        leaf_nodes += perft_bulk(successor::make_copy(p, m), depth - 1, ply + 1);
                return leaf_nodes;
        }

        NodeCount perft_count(Position const& p, int depth, int ply)
        {
                BOOST_ASSERT(0 < depth);
                statistics_.update(ply);

                if (depth == 1)
                        return successor::count(p);

                Arena<Move> a;
                auto const moves = successor::generate(p, a);
                NodeCount leaf_nodes = 0;
                for (auto const& m: moves)
                        leaf_nodes += perft_count(successor::make_copy(p, m), depth - 1, ply + 1);
                return leaf_nodes;
        }

        NodeCount perft_hash(Position const& p, int depth, int ply)
        {
                BOOST_ASSERT(0 < depth);
                statistics_.update(ply);

                auto const TT_entry = TT.find(p);
                if (TT_entry && TT_entry->depth() == depth)
                        return TT_entry->nodes();

                NodeCount leaf_nodes;
                if (depth == 1) {
                        leaf_nodes = successor::count(p);
                } else {
                        Arena<Move> a;
                        auto const moves = successor::generate(p, a);
                        leaf_nodes = std::accumulate(std::begin(moves), std::end(moves), NodeCount(0), [=, &p](NodeCount sub, Move const& m){
                                return sub + this->perft_hash(successor::make_copy(p, m), depth - 1, ply + 1);
                        });
                }

                TT.emplace(p, leaf_nodes, depth);
                return leaf_nodes;
        }

        // 32-byte hash entries: 24-byte piece lists signature, 8-byte (59-bit leafs, 5-bit depth) content
        // 8-way buckets on 64-byte cache lines, (2^25 entries = 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::DualMap<
                Position, 
                Transposition, 
                hash::MaterialExtractor
        > TranspositionTable;

        typedef hash::Map< 
                Position, 
                Transposition, 
                hash::UpperHashBitsExtractor, 
                hash::EmptyOldUnderCutMin<hash::Smallest> 
        > TranspositionTable2;

        TranspositionTable2 TT;

        // representation
        Statistics statistics_;
};

}       // namespace walk
}       // namespace dctl
