#pragma once
#include <cstddef>                      // size_t
#include <iomanip>
#include <iostream>
#include <iterator>                     // begin, end
#include <boost/assert.hpp>
#include "Transposition.hpp"
#include "../hash/DualMap.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../successor/Successor.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/Statistics.hpp"
#include "../utility/Timer.hpp"

#include "../setup/Diagram.hpp"
#include "../setup/String.hpp"
#include "../notation/String.hpp"

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
                        leafs = driver(p, 0, i);
                        timer.split();
                        report(i, leafs, timer);
                }
                summary();
                return leafs;
        }

        template<template<typename, typename> class Position, typename Rules, typename Board>
        NodeCount divide(Position<Rules, Board> const& p, int depth)
        {
                NodeCount leafs = 0;
                NodeCount move_leafs;

                Timer timer;
                Stack moves;
                Successor<select::Legal>::generate(p, moves);

                announce(p, depth, moves.size());
                for (auto i = 0; i < static_cast<int>(moves.size()); ++i) {
                        statistics_.reset();
                        print_move(notation::write<Rules>()(p, moves[i]), i);

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
                return TT.resize(s);
        }

        void clear_hash()
        {
                return TT.clear();
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
                std::cout << std::setw( 6) << timer.elapsed();

                double const nps = (1000 * statistics_.nodes()) / static_cast<double>(timer.lap());
                std::cout << " nps ";
                std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
                std::cout << std::setw( 7) << nps;

                double const hashfull = 1000 * (static_cast<double>((TT.size() - TT.available())) / TT.size());
                std::cout << " hashfull ";
                std::cout << std::setw( 4) << std::right << hashfull;

                std::cout << "\n";
        }

        void summary() const
        {
                std::cout << "\n";
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
                return (depth == 0)? leaf(p, depth, ply) : fast(p, depth, ply);
        }

        template<typename Position>
        NodeCount leaf(Position const& p, int depth, int ply)
        {
                statistics_.update(ply);

                if (depth == 0)
                        return 1;

                Stack moves;
                Successor<select::Legal>::generate(p, moves);
                NodeCount leafs = 0;
                for (auto m = std::begin(moves); m != std::end(moves); ++m) {
                        auto q = p;
                        q.attach(p);
                        q.make(*m);
                        leafs += leaf(q, depth - 1, ply + 1);
                }
                return leafs;
        }

        template<typename Position>
        NodeCount bulk(Position const& p, int depth, int ply)
        {
                statistics_.update(ply);

                Stack moves;
                Successor<select::Legal>::generate(p, moves);
                if (depth == 1)
                        return moves.size();

                NodeCount leafs = 0;
                for (auto m = std::begin(moves); m != std::end(moves); ++m) {
                        auto q = p;
                        q.attach(p);
                        q.make(*m);
                        leafs += bulk(q, depth - 1, ply + 1);
                }
                return leafs;
        }

        template<typename Position>
        NodeCount count(Position const& p, int depth, int ply)
        {
                statistics_.update(ply);

                if (depth == 1)
                        return Successor<select::Legal>::count(p);

                Stack moves;
                Successor<select::Legal>::generate(p, moves);
                NodeCount leafs = 0;
                for (auto m = std::begin(moves); m != std::end(moves); ++m) {
                        auto q = p;
                        q.attach(p);
                        q.make(*m);
                        leafs += count(q, depth - 1, ply + 1);
                }
                return leafs;
        }

        template<typename Position>
        NodeCount hash(Position const& p, int depth, int ply)
        {
                statistics_.update(ply);

                auto TT_entry = TT.find(p);
                if (TT_entry && TT_entry->depth() == depth)
                        return TT_entry->leafs();

                if (depth == 0)
                        return 1;

                Stack moves;
                Successor<select::Legal>::generate(p, moves);
                NodeCount leafs = 0;
                for (auto m = std::begin(moves); m != std::end(moves); ++m) {
                        auto q = p;
                        q.attach(p);
                        q.make(*m);
                        leafs += hash(q, depth - 1, ply + 1);
                }

                TT.insert(p, Transposition(leafs, depth));
                return leafs;
        }

        template<typename Position>
        NodeCount fast(Position const& p, int depth, int ply)
        {
                statistics_.update(ply);

                auto TT_entry = TT.find(p);
                if (TT_entry && TT_entry->depth() == depth)
                        return TT_entry->leafs();

                NodeCount leafs;
                if (depth == 1) {
                        leafs = Successor<select::Legal>::count(p);
                } else {
                        Stack moves;
                        Successor<select::Legal>::generate(p, moves);
                        leafs = 0;
                        for (auto m = std::begin(moves); m != std::end(moves); ++m) {
                                auto q = p;
                                q.attach(p);
                                q.make(*m);
                                leafs += fast(q, depth - 1, ply + 1);
                        }
                }

                TT.insert(p, Transposition(leafs, depth));
                return leafs;
        }

        // 32-byte hash entries: 24-byte piece lists signature, 8-byte (59-bit leafs, 5-bit depth) content
        // 2-way buckets on 64-byte cache lines, 2 * 2^23 buckets (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::DualMap<Material, Transposition> TranspositionTable;
        TranspositionTable TT;

        // representation
        Statistics statistics_;
};

}       // namespace walk
}       // namespace dctl
