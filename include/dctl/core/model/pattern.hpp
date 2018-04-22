#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model/stride.hpp>           // next, prev
#include <dctl/core/rules/type_traits.hpp>      // short_ranged_tag, long_ranged_tag
#include <dctl/util/type_traits.hpp>            // set_t

namespace dctl::core {

template<class Board, class Direction>
struct move_squares
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& squares) const noexcept
        {
                return next<Board, Direction>{}(sources) & squares;
        }
};

template<class Board, class Direction>
struct jump_sources
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& squares) const noexcept
        {
                return sources & prev<Board, Direction>{}(targets) & prev<Board, Direction, 2>{}(squares);
        }

        auto operator()(set_type const& targets, set_type const& squares) const noexcept
        {
                return prev<Board, Direction>{}(targets) & prev<Board, Direction, 2>{}(squares);
        }
};

template<class Board, class Direction>
struct jump_targets
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& squares) const noexcept
        {
                return next<Board, Direction>{}(sources) & targets & prev<Board, Direction>{}(squares);
        }

        auto operator()(set_type const& targets, set_type const& squares) const noexcept
        {
                return targets & prev<Board, Direction>{}(squares);
        }
};

}       // namespace dctl::core
