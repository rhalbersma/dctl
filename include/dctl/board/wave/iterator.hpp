#pragma once
#include <dctl/board/wave/cursor.hpp>           // cursor
#include <dctl/utility/type_traits.hpp>         // set_t
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <iterator>                             // bidirectional_iterator_tag

namespace dctl {
namespace board {
namespace wave {

template<class Board, int Direction>
using iterator = boost::counting_iterator
<
        cursor<Board, Direction>,               // Incrementable
        std::bidirectional_iterator_tag,        // CategoryOrTraversal
        int                                     // DifferenceType
>;

template<class Board, int Direction, class Set = set_t<Board>>
auto make_iterator(Set const s)
        -> iterator<Board, Direction>
{
        return { cursor<Board, Direction>{s} };
}

}       // namespace wave
}       // namespace board
}       // namespace dctl
