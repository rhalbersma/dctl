#pragma once
#include <iterator>                             // random_access_iterator_tag
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <dctl/ray/cursor.hpp>                  // StridedCursor

namespace dctl {
namespace ray {

template<class Board, int Direction>
using RayIterator = boost::counting_iterator
<
        StridedCursor<Board, Direction>,        // Incrementable
        std::random_access_iterator_tag,        // CategoryOrTraversal
        int                                     // DifferenceType
>;

}       // namespace ray
}       // namespace dctl
