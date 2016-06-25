#pragma once
#include <dctl/mask/detail/iterator.hpp>        // make_iterator
#include <dctl/rule_traits.hpp>                 // short_ranged_tag
#include <iterator>                             // prev

namespace dctl {
namespace mask {

template<class Board, int Direction, class KingRangeCategory>
struct push_sources;

template<class Board, int Direction>
struct push_sources<Board, Direction, short_ranged_tag>
{
        template<class Set>
        auto operator()(Set const active_pieces, Set const not_occupied) const noexcept
        {
                return active_pieces & Set(*std::prev(detail::make_iterator<Board, Direction>(not_occupied)));
        }
};

}       // namespace mask
}       // namespace dctl
