#pragma once
#include <dctl/board/ray/cursor.hpp>            // cursor
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <cstddef>                              // size_t
#include <iterator>                             // random_access_iterator_tag

namespace dctl {
namespace board {
namespace ray {

template<class Board, int Direction>
using iterator = boost::counting_iterator
<
        cursor<Board, Direction>,               // Incrementable
        std::random_access_iterator_tag,        // CategoryOrTraversal
        int                                     // DifferenceType
>;

template<class Board, int Direction>
iterator<Board, Direction> make_iterator(std::size_t const sq)
{
        return { cursor<Board, Direction>{sq} };
}

}       // namespace ray
}       // namespace board
}       // namespace dctl
