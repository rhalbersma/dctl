#pragma once
#include <dctl/angle.hpp>               // Angle, rotate, mirror
#include <dctl/ray/iterator.hpp>        // Iterator

namespace dctl {
namespace ray {

template<int Theta, class Board, int Direction>
auto rotate(Iterator<Board, Direction> it)
-> Iterator<Board, rotate(Angle{Direction}, Angle{Theta})>
{
        return { it.base() };
}

template<int Theta, class Board, int Direction>
auto mirror(Iterator<Board, Direction> it)
-> Iterator<Board, mirror(Angle{Direction}, Angle{Theta})>
{
        return { it.base() };
}

}       // namespace ray
}       // namespace dctl
