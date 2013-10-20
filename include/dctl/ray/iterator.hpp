#pragma once
#include <iterator>                     // random_access_iterator_tag
#include <boost/iterator.hpp>           // counting_iterator
#include <dctl/ray/cursor.hpp>          // StridedCursor

namespace dctl {
namespace ray {

template<class Board, int Direction>
using SquareIterator = boost::counting_iterator
<
        StridedCursor<Board::shift_size(Direction)>,
        std::random_access_iterator_tag
>;

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
