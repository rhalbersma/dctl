#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model/detail/stride.hpp>    // next, prev
#include <dctl/util/type_traits.hpp>            // set_t

namespace dctl::core {

template<class Board, class Direction>
struct move_dest
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& from, set_type const& dest) const noexcept
        {
                return next<Board, Direction>{}(from) & dest;
        }
};

template<class Board, class Direction>
struct jump_from
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& from, set_type const& targets, set_type const& dest) const noexcept
        {
                return from & prev<Board, Direction>{}(targets) & prev<Board, Direction, 2>{}(dest);
        }

        auto operator()(set_type const& targets, set_type const& dest) const noexcept
        {
                return prev<Board, Direction>{}(targets) & prev<Board, Direction, 2>{}(dest);
        }
};

template<class Board, class Direction>
struct jump_targets
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& from, set_type const& targets, set_type const& dest) const noexcept
        {
                return next<Board, Direction>{}(from) & targets & prev<Board, Direction>{}(dest);
        }

        auto operator()(set_type const& targets, set_type const& dest) const noexcept
        {
                return targets & prev<Board, Direction>{}(dest);
        }
};

}       // namespace dctl::core
