#pragma once
#include <iterator>                     // random_access_iterator_tag
#include <boost/iterator.hpp>           // counting_iterator
#include <dctl/ray/cursor.hpp>          // StridedCursor

namespace dctl {
namespace ray {

template<int N>
using StridedIterator = boost::counting_iterator
<
        StridedCursor<N>,
        std::random_access_iterator_tag
>;

}       // namespace ray
}       // namespace dctl
