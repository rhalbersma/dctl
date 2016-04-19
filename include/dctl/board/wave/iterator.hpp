#pragma once
#include <dctl/board/wave/cursor.hpp>           // Cursor
#include <dctl/utility/type_traits.hpp>         // set_t
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

template<class Board, int Direction, class Set = set_t<Board>>
Iterator<Board, Direction> make_iterator(Set const& s)
{
        return { Cursor<Board, Direction>{s} };
}

}       // namespace wave
}       // namespace dctl
