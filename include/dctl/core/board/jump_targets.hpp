#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/mask_fill.hpp>        // fill
#include <dctl/core/board/mask_iterator.hpp>    // make_iterator
#include <dctl/core/board/shift.hpp>            // next, prev
#include <dctl/util/type_traits.hpp>            // set_t
#include <dctl/core/rules/type_traits.hpp>      // short_ranged_tag, long_ranged_tag
#include <iterator>                             // next, prev

namespace dctl::core {

template<class Board, int Direction>
struct jump_sources
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& empty) const
        {
                return sources & prev<Board, Direction, 1>{}(targets) & prev<Board, Direction, 2>{}(empty);
        }
};

template<class Board, int Direction, class KingRangeCategory = short_ranged_tag>
struct jump_targets
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& empty) const
        {
                return next<Board, Direction>{}(sources) & targets & prev<Board, Direction>{}(empty);
        }
};

template<class Board, int Direction>
struct jump_targets<Board, Direction, long_ranged_tag>
{
        template<class Set>
        auto operator()(Set const active_pieces, Set const passive_pieces, Set const not_occupied) const
        {
                return
                        Set(*std::next(mask::make_iterator<Board, Direction>(mask::fill<Board, Direction>{}(active_pieces, not_occupied)))) &
                        passive_pieces &
                        Set(*std::prev(mask::make_iterator<Board, Direction>(not_occupied)))
                ;
        }
};

}       // namespace dctl::core
