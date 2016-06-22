#pragma once
#include <dctl/rule_traits.hpp>         // short_ranged_tag, long_ranged_tag
#include <dctl/board/algorithm.hpp>     // Fill
#include <dctl/board/wave/iterator.hpp> // make_iterator
#include <iterator>                     // next, prev

namespace dctl {
namespace board {

template<class Board, int Direction, class KingRangeCategory>
struct push_targets;

template<class Board, int Direction>
struct push_targets<Board, Direction, short_ranged_tag>
{
        template<class Set>
        auto operator()(Set const active_pieces, Set const not_occupied) const
        {
                return Set(*std::next(wave::make_iterator<Board, Direction>(active_pieces))) & not_occupied;
        }
};

template<class Board, int Direction>
struct push_targets<Board, Direction, long_ranged_tag>
{
        template<class Set>
        auto operator()(Set const active_pieces, Set const not_occupied) const
        {
                return active_pieces ^ Fill<Board, Direction>{}(active_pieces, not_occupied);
        }
};

template<class Board, int Direction, class KingRangeCategory>
class jump_sources;

template<class Board, int Direction>
class jump_sources<Board, Direction, short_ranged_tag>
{
        template<class Set>
        auto prev_set(Set const s) const
        {
                return Set(*std::prev(wave::make_iterator<Board, Direction>(s)));
        }
public:
        template<class Set>
        auto operator()(Set const active_pieces, Set const passive_pieces, Set const not_occupied) const
        {
                return active_pieces & prev_set(passive_pieces & prev_set(not_occupied));
        }
};

template<class Board, int Direction, class KingRangeCategory>
class jump_targets;

template<class Board, int Direction>
class jump_targets<Board, Direction, short_ranged_tag>
{
        template<class Set>
        auto next_set(Set const s) const
        {
                return Set(*std::next(wave::make_iterator<Board, Direction>(s)));
        }

        template<class Set>
        auto prev_set(Set const s) const
        {
                return Set(*std::prev(wave::make_iterator<Board, Direction>(s)));
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
                        Set(*std::next(wave::make_iterator<Board, Direction>(Fill<Board, Direction>{}(active_pieces, not_occupied)))) &
                        passive_pieces &
                        Set(*std::prev(wave::make_iterator<Board, Direction>(not_occupied)))
                ;
        }
};

}       // namespace board
}       // namespace dctl
