#pragma once
#include <iterator>                             // bidirectional_iterator_tag
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <dctl/wave/cursor.hpp>                 // Cursor

namespace dctl {
namespace wave {

template<class Board, int Direction>
using Iterator = boost::counting_iterator
<
        Cursor<Board, Direction>,               // Incrementable
        std::bidirectional_iterator_tag,        // CategoryOrTraversal
        int                                     // DifferenceType
>;

template<class Board, int Direction, class Set = typename Board::set_type>
Iterator<Board, Direction> make_iterator(Set const& s)
{
        return { Cursor<Board, Direction>{s} };
}

}       // namespace ray
}       // namespace dctl
