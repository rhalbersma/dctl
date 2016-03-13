#pragma once
#include <dctl/board/angle.hpp>         // angle, rotate, mirror
#include <dctl/board/ray/iterator.hpp>  // Iterator

namespace dctl {
namespace ray {

template<int Theta, class Board, int Direction>
auto rotate(Iterator<Board, Direction> it)
        -> Iterator<Board, rotate(angle{Direction}, angle{Theta}).degrees()>
{
        return { it.base() };
}

template<int Theta, class Board, int Direction>
auto mirror(Iterator<Board, Direction> it)
        -> Iterator<Board, mirror(angle{Direction}, angle{Theta}).degrees()>
{
        return { it.base() };
}

template<int Theta, class Board, int Direction>
auto turn(Iterator<Board, Direction> it)
        -> Iterator<Board, Theta>
{
        static_assert(Theta != Direction);
        return { it.base() };
}

}       // namespace ray
}       // namespace dctl
