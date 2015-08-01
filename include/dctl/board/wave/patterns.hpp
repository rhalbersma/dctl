#pragma once
#include <dctl/rule_traits.hpp>
#include <dctl/board/wave/algorithm.hpp>      // Fill
#include <dctl/board/wave/iterator.hpp>       // make_iterator
#include <iterator>                     // next, prev
#include <type_traits>                  // false_type, true_type

namespace dctl {

template<class Board, int Direction, class IsLongRangedKing>
struct Sink;

template<class Board, int Direction>
struct Sink<Board, Direction, short_ranged_tag>
{
        template<class Set>
        auto operator()(Set const& from, Set const& dest) const
        {
                return Set(*std::next(wave::make_iterator<Board, Direction>(from))) & dest;
        }
};

template<class Board, int Direction>
struct Sink<Board, Direction, long_ranged_tag>
{
        template<class Set>
        auto operator()(Set const& from, Set const& dest) const
        {
                return from ^ Fill<Board, Direction>{}(from, dest);
        }
};

template<class Board, int Direction, class IsLongRangedKing>
struct Sandwich;

template<class Board, int Direction>
struct Sandwich<Board, Direction, short_ranged_tag>
{
        template<class Set>
        auto operator()(Set const& from, Set const& through, Set const& dest) const
        {
                return
                        Set(*std::next(wave::make_iterator<Board, Direction>(from))) &
                        through &
                        Set(*std::prev(wave::make_iterator<Board, Direction>(dest)))
                ;
        }
};

template<class Board, int Direction>
struct Sandwich<Board, Direction, long_ranged_tag>
{
        template<class Set>
        auto operator()(Set const& from, Set const& through, Set const& dest) const
        {
                return
                        Set(*std::next(wave::make_iterator<Board, Direction>(Fill<Board, Direction>{}(from, dest)))) &
                        through &
                        Set(*std::prev(wave::make_iterator<Board, Direction>(dest)))
                ;
        }
};

}       // namespace dctl
