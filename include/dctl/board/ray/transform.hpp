#pragma once
#include <dctl/board/angle.hpp>         // Angle, rotate, mirror
#include <dctl/board/ray/iterator.hpp>  // Iterator

namespace dctl {
namespace board {
namespace ray {

template<int Theta, class Board, int Direction>
auto rotate(iterator<Board, Direction> it)
        -> iterator<Board, rotate(Angle{Direction}, Angle{Theta}).degrees()>
{
        return { it.base() };
}

template<int Theta, class Board, int Direction>
auto mirror(iterator<Board, Direction> it)
        -> iterator<Board, mirror(Angle{Direction}, Angle{Theta}).degrees()>
{
        return { it.base() };
}

template<int Theta, class Board, int Direction>
auto turn(iterator<Board, Direction> it)
        -> iterator<Board, Theta>
{
        static_assert(Theta != Direction);
        return { it.base() };
}

}       // namespace ray
}       // namespace board
}       // namespace dctl
