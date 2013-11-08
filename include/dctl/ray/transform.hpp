#pragma once
#include <dctl/ray/iterator.hpp>        // RayIterator
#include <dctl/angle.hpp>               // Angle, rotate

namespace dctl {
namespace ray {

template<int Theta, class Board, int Direction>
auto rotate(RayIterator<Board, Direction> it)
-> RayIterator<Board, rotate(Angle{Direction}, Angle{Theta})>
{
        return {it.base()};
}

}       // namespace ray
}       // namespace dctl
