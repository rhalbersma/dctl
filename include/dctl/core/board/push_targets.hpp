#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask_fill.hpp>        // fill
#include <dctl/core/board/mask_iterator.hpp>    // make_iterator
#include <dctl/core/rules/type_traits.hpp>           // short_ranged_tag, long_ranged_tag
#include <iterator>                             // next

namespace dctl::core {

template<class Board, int Direction, class KingRangeCategory>
struct push_targets;

template<class Board, int Direction>
struct push_targets<Board, Direction, short_ranged_tag>
{
        template<class Set>
        auto operator()(Set const active_pieces, Set const not_occupied) const noexcept
        {
                return Set(*std::next(mask::make_iterator<Board, Direction>(active_pieces))) & not_occupied;
        }
};

template<class Board, int Direction>
struct push_targets<Board, Direction, long_ranged_tag>
{
        template<class Set>
        auto operator()(Set const active_pieces, Set const not_occupied) const noexcept
        {
                return active_pieces ^ mask::fill<Board, Direction>{}(active_pieces, not_occupied);
        }
};

}       // namespace dctl::core
