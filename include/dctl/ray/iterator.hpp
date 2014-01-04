#pragma once
#include <iterator>                             // random_access_iterator_tag
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <dctl/ray/cursor.hpp>                  // Cursor

namespace dctl {
namespace ray {

template<class Board, int Direction>
using Iterator = boost::counting_iterator
<
        Cursor<Board, Direction>,               // Incrementable
        std::random_access_iterator_tag,        // CategoryOrTraversal
        int                                     // DifferenceType
>;

template<class Board, int Direction>
Iterator<Board, Direction> make_iterator(int sq)
{
        return { Cursor<Board, Direction>{sq} };
}

}       // namespace ray
}       // namespace dctl
