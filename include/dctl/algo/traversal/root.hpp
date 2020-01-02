#pragma once

//          Copyright Rein Halbersma 2010-2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/node.hpp>
#include <dctl/algo/traversal/transposition.hpp>
#include <dctl/core.hpp>
#include <dctl/util/hash/dual_map.hpp>
#include <dctl/util/hash/extract.hpp>
#include <dctl/util/statistics.hpp>
#include <dctl/util/stopwatch.hpp>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>                     // distance
#include <memory>
#include <numeric>
#include <utility>
#include <map>
#include <unordered_map>

namespace dctl::algo {
namespace traversal {

template<bool IsBulk, class Model, class State>
auto depth_limited_count(Model const& m, State const& s, int depth)
        -> int64_t
{
        if constexpr(IsBulk) { if (depth == 1) { return m.count(s); } }
        else                 { if (depth == 0) { return 1;          } }

        auto const actions = m.generate(s);
        return std::accumulate(actions.cbegin(), actions.cend(), int64_t{0}, [&](auto sum, auto const& a) {
                return sum + depth_limited_count<IsBulk>(m, result(s, a), depth - 1);
        });
}

template<class State>
void announce(State const& s, int depth)
{
        std::cout << core::diag << s;
        std::cout << core::fen << s << '\n';
        std::cout << "Searching to nominal depth=" << depth << "\n\n";
}

template<class State>
void announce(State const& s, int depth, int num_moves)
{
        announce(s, depth);
        std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << depth - 1 << '\n';
        std::cout << '\n';
}

template<class Action>
void print_move(Action const& move, int i)
{
        std::cout << std::setw(2) << (i + 1) << "." << move << " ";
}

template<class Stopwatch>
void report(int depth, int64_t leafs, Stopwatch const& stopwatch)
{
        std::cout << "info";

        std::cout << " depth ";
        std::cout << std::setw( 2) << depth;

        std::cout << " leafs ";
        std::cout << std::setw(12) << std::right << leafs;

        auto const lap = stopwatch.lap_time();
        std::cout << " time ";
        std::cout << std::setw( 6) << lap.count();

        auto const nps = static_cast<double>(leafs) / static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(lap).count());
        std::cout << " nps ";
        std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
        std::cout << std::setw( 7) << nps;
/*
        double const hashfull = 1000 * (static_cast<double>(e.handle_->TT_.size()) / static_cast<double>(e.handle_->TT_.capacity()));
        std::cout << " hashfull ";
        std::cout << std::setw( 4) << std::right << hashfull;
*/
        std::cout << std::endl;
}

template<class Stopwatch>
void xreport(int depth, int64_t leafs, Stopwatch const& stopwatch)
{
        std::cout << "info";

        std::cout << " depth ";
        std::cout << std::setw( 2) << depth;

        std::cout << " leafs ";
        std::cout << std::setw(12) << std::right << leafs;

        auto const lap = stopwatch.lap_time();
        std::cout << " time ";
        std::cout << std::setw( 6) << lap.count();

        auto const knps = static_cast<double>(leafs) / static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(lap).count());
        std::cout << " " << knps << " knps";

/*
        double const hashfull = 1000 * (static_cast<double>(e.handle_->TT_.size()) / static_cast<double>(e.handle_->TT_.capacity()));
        std::cout << " hashfull ";
        std::cout << std::setw( 4) << std::right << hashfull;
*/
        std::cout << std::endl;
}

inline
void summary(int64_t leafs)
{
        std::cout << "Total leafs: " << leafs << "\n\n";
}

template<class Actions, class State>
auto sperft(Actions successor, State const& s, int depth)
{
        announce(s, depth);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto d = 1; d <= depth; ++d) {
                stopwatch.split_reset();
                auto const nodes = depth_limited_count<true>(successor, s, d);
                stopwatch.split_reset();
                xreport(d, nodes, stopwatch);
        }
}

template<class Actions, class State>
auto divide(Actions successor, State const& s, int depth)
{
        int64_t leaf_nodes = 0;

        auto moves = successor.generate(s);
        std::sort(moves.begin(), moves.end(), [](auto const& lhs, auto const& rhs) {
                return str_numeric(lhs) < str_numeric(rhs);
        });

        announce(s, depth, static_cast<int>(moves.size()));
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto&& a : moves) {
                auto const i = static_cast<int>(std::distance(&moves[0], &a));
                print_move(a, i);
                auto const sub_count = depth_limited_count<false>(successor, result(s, a), depth - 1);
                leaf_nodes += sub_count;
                stopwatch.split_reset();
                report(depth - 1, sub_count, stopwatch);
        }
        summary(leaf_nodes);
        return leaf_nodes;
}

}       // namespace traversal
}       // namespace dctl::algo
