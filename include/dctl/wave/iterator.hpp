#pragma once
#include <dctl/set_type.hpp>                    // set_type
#include <dctl/wave/cursor.hpp>                 // Cursor
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <iterator>                             // bidirectional_iterator_tag

namespace dctl {
namespace wave {

template<class Board, int Direction>
using Iterator = boost::counting_iterator
<
        Cursor<Board, Direction>,               // Incrementable
        std::bidirectional_iterator_tag,        // CategoryOrTraversal
        int                                     // DifferenceType
>;

template<class Board, int Direction, class Set = set_type<Board>>
Iterator<Board, Direction> make_iterator(Set const& s)
{
        return { Cursor<Board, Direction>{s} };
}

}       // namespace ray
}       // namespace dctl
