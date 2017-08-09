#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/actions/detail/stride.hpp>  // stride_v
#include <dctl/core/rules/type_traits.hpp>      // short_ranged_tag, long_ranged_tag
#include <dctl/util/type_traits.hpp>            // set_t

namespace dctl::core {

template<class Board, int Direction>
struct move_sources
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& squares) const noexcept
        {
                return sources & prev<Board, Direction>{}(squares);
        }
};

template<class Board, int Direction>
struct move_squares
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& squares) const noexcept
        {
                return next<Board, Direction>{}(sources) & squares;
        }
};

template<class Board, int Direction>
struct jump_sources
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& squares) const noexcept
        {
                return sources & prev<Board, Direction, 1>{}(targets) & prev<Board, Direction, 2>{}(squares);
        }
};

template<class Board, int Direction, class KingRangeCategory = short_ranged_tag>
struct jump_targets
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& squares) const noexcept
        {
                return next<Board, Direction, 1>{}(sources) & targets & prev<Board, Direction, 1>{}(squares);
        }
};

template<class Board, int Direction, class KingRangeCategory = short_ranged_tag>
struct jump_squares
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& squares) const noexcept
        {
                return next<Board, Direction, 2>{}(sources) & next<Board, Direction, 1>{}(targets) & squares;
        }
};

template<class Board, int Direction>
struct fill
{
        using set_type = set_t<Board>;

        constexpr auto operator()(set_type generator, set_type const& propagator) const noexcept
        {
                auto flood = set_type{};
                while (!generator.empty()) {
                        flood |= generator;
                        generator = next<Board, Direction>{}(generator) & propagator;
                }
                return flood;
        }
};

template<class Board, int Direction>
struct jump_targets<Board, Direction, long_ranged_tag>
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& empty) const noexcept
        {
                return
                        next<Board, Direction>{}(fill<Board, Direction>{}(sources, empty)) &
                        targets &
                        prev<Board, Direction>{}(empty)
                ;
        }
};

}       // namespace dctl::core
