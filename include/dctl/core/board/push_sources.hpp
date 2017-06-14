#pragma once
#include <dctl/core/board/mask_iterator.hpp>    // make_iterator
#include <dctl/core/rules/type_traits.hpp>           // short_ranged_tag
#include <iterator>                             // prev

namespace dctl::core {

template<class Board, int Direction, class KingRangeCategory>
struct push_sources;

template<class Board, int Direction>
struct push_sources<Board, Direction, short_ranged_tag>
{
        template<class Set>
        auto operator()(Set const active_pieces, Set const not_occupied) const noexcept
        {
                return active_pieces & Set(*std::prev(mask::make_iterator<Board, Direction>(not_occupied)));
        }
};

}       // namespace dctl::core
