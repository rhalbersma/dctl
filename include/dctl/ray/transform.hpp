#pragma once
#include <dctl/ray/iterator.hpp>        // RayIterator
#include <dctl/angle/transform.hpp>     // rotate, mirror

namespace dctl {
namespace ray {

template<int Theta, class Board, int Direction>
auto rotate(RayIterator<Board, Direction> const& it)
-> RayIterator<Board, rotate(Angle{Direction}, Angle{Theta})>
{
        return {it.base()};
}

}       // namespace ray
}       // namespace dctl
