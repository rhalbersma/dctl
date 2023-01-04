#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

namespace dctl::algo::traversal {

template<bool IsBulk>
[[nodiscard]] constexpr auto depth_limited_count(auto const& state, int depth, auto const& model) noexcept
        -> int64_t
{
        if constexpr(IsBulk) { if (depth == 1) { return model.count(state); } }
        else                 { if (depth == 0) { return 1;                  } }

        auto const actions = model.generate(state);
        return std::accumulate(actions.cbegin(), actions.cend(), int64_t(0), [&](auto sum, auto const& action) {
                return sum + depth_limited_count<IsBulk>(result(state, action), depth - 1, model);
        });
}

void announce(auto const& state, int depth)
{
        std::cout << core::diag << state;
        std::cout << core::fen << state << '\n';
        std::cout << "Searching to nominal depth=" << depth << "\n\n";
}

void announce(auto const& state, int depth, int num_actions)
{
        announce(state, depth);
        std::cout << "Found " << num_actions << " actions, searching each to nominal depth=" << depth - 1 << '\n';
        std::cout << '\n';
}

void print_move(int index, auto const& action)
{
        std::cout << std::setw(2) << index << "." << action << " ";
}

void report(int depth, int64_t leafs, auto const& stopwatch)
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

inline void summary(int64_t leafs)
{
        std::cout << "Total leafs: " << leafs << "\n\n";
}

constexpr auto perft(auto const& state, int depth_limit, auto const& model)
{
        announce(state, depth_limit);
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto depth = 1; depth <= depth_limit; ++depth) {
                stopwatch.split_reset();
                auto const nodes = depth_limited_count<true>(state, depth, model);
                stopwatch.split_reset();
                report(depth, nodes, stopwatch);
        }
}

auto divide(auto const& state, int depth, auto const& model)
{
        int64_t leaf_nodes = 0;

        auto actions = model.generate(state);
        std::ranges::sort(actions, [](auto const& lhs, auto const& rhs) {
                return str_numeric(lhs) < str_numeric(rhs);
        });

        announce(state, depth, static_cast<int>(actions.size()));
        util::Stopwatch stopwatch;
        stopwatch.start_stop();
        for (auto index = 1; auto const& action : actions) {
                print_move(index++, action);
                auto const sub_count = depth_limited_count<true>(result(state, action), depth - 1, model);
                leaf_nodes += sub_count;
                stopwatch.split_reset();
                report(depth - 1, sub_count, stopwatch);
        }
        summary(leaf_nodes);
        return leaf_nodes;
}

}       // namespace dctl::algo::traversal
