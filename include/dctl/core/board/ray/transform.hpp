#pragma once
#include <dctl/core/board/angle.hpp>         // angle, rotate, mirror
#include <dctl/core/board/ray/iterator.hpp>  // Iterator

namespace dctl {
namespace core {
namespace board {
namespace ray {

template<int Theta, class Board, int Direction>
auto rotate(iterator<Board, Direction> it)
        -> iterator<Board, rotate(angle{Direction}, angle{Theta}).value()>
{
        return { it.base() };
}

template<int Theta, class Board, int Direction>
auto mirror(iterator<Board, Direction> it)
        -> iterator<Board, mirror(angle{Direction}, angle{Theta}).value()>
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
}       // namespace core
}       // namespace dctl
