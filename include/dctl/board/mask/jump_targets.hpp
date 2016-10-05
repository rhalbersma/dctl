#pragma once
#include <dctl/board/mask/detail/fill.hpp>      // fill
#include <dctl/board/mask/detail/iterator.hpp>  // make_iterator
#include <dctl/rule_traits.hpp>                 // short_ranged_tag, long_ranged_tag
#include <iterator>                             // next, prev

namespace dctl {
namespace board {
namespace mask {

template<class Board, int Direction, class KingRangeCategory>
class jump_targets;

template<class Board, int Direction>
class jump_targets<Board, Direction, short_ranged_tag>
{
        template<class Set>
        auto next_set(Set const s) const
        {
                return Set(*std::next(detail::make_iterator<Board, Direction>(s)));
        }

        template<class Set>
        auto prev_set(Set const s) const
        {
                return Set(*std::prev(detail::make_iterator<Board, Direction>(s)));
        }
public:
        template<class Set>
        auto operator()(Set const active_pieces, Set const passive_pieces, Set const not_occupied) const
        {
                return next_set(active_pieces) & passive_pieces & prev_set(not_occupied);
        }
};

template<class Board, int Direction>
class jump_targets<Board, Direction, long_ranged_tag>
{
public:
        template<class Set>
        auto operator()(Set const active_pieces, Set const passive_pieces, Set const not_occupied) const
        {
                return
                        Set(*std::next(detail::make_iterator<Board, Direction>(detail::fill<Board, Direction>{}(active_pieces, not_occupied)))) &
                        passive_pieces &
                        Set(*std::prev(detail::make_iterator<Board, Direction>(not_occupied)))
                ;
        }
};

}       // namespace mask
}       // namespace board
}       // namespace dctl
