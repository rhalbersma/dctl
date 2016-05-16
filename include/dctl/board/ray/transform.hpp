#pragma once
#include <dctl/board/angle.hpp>         // angle, rotate, mirror
#include <dctl/board/ray/iterator.hpp>  // Iterator

namespace dctl {
namespace board {
namespace ray {

template<int Theta, class Board, int Direction>
auto rotate(iterator<Board, Direction> it)
        -> iterator<Board, rotate(angle{Direction}, angle{Theta}).degrees()>
{
        return { it.base() };
}

template<int Theta, class Board, int Direction>
auto mirror(iterator<Board, Direction> it)
        -> iterator<Board, mirror(angle{Direction}, angle{Theta}).degrees()>
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
