#pragma once
#include <dctl/ray/iterator.hpp>        // SquareIterator
#include <dctl/angle/transform.hpp>     // rotate, mirror

namespace dctl {
namespace ray {

template<class Board, int Direction, class T>
auto rotate(SquareIterator<Board, Direction> it, T const& theta)
{
        return SquareIterator<Board, angle::rotate(Direction, theta)>{it.base()};
}

template<class Board, int Direction, class T>
auto mirror(SquareIterator<Board, Direction> it, T const& theta)
{
        return SquareIterator<Board, angle::mirror(Direction, theta)>{it.base()};
}

}       // namespace ray
}       // namespace dctl
