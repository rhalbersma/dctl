#pragma once
#include <dctl/board/algorithm.hpp>     // Fill
#include <dctl/board/iterator.hpp>      // Next, Prev
#include <dctl/rules/types.hpp>          // range::distance_1, range::distance_N

namespace dctl {

// primary template
template<class Board, int Direction, class Range>
struct Sink;

template<class Board, int Direction>
struct Sink<Board, Direction, rules::range::distance_1>
{
        template<class T>
        T operator()(T from, T dest) const
        {
                return Next<Board, Direction>()(from) & dest;
        }
};

template<class Board, int Direction>
struct Sink<Board, Direction, rules::range::distance_N>
{
        template<class T>
        T operator()(T from, T dest) const
        {
                return from ^ Fill<Board, Direction>()(from, dest);
        }
};

// primary template
template<class Board, int Direction, class Range>
struct Sandwich;

template<class Board, int Direction>
struct Sandwich<Board, Direction, rules::range::distance_1>
{
        template<class T>
        T operator()(T from, T through, T dest) const
        {
                return (
                        Next<Board, Direction>()(from) &
                        through &
                        Prev<Board, Direction>()(dest)
                );
        }
};

template<class Board, int Direction>
struct Sandwich<Board, Direction, rules::range::distance_N>
{
        template<class T>
        T operator()(T from, T through, T dest) const
        {
                return (
                        Next<Board, Direction>()(Fill<Board, Direction>()(from, dest)) &
                        through &
                        Prev<Board, Direction>()(dest)
                );
        }
};

}       // namespace dctl
