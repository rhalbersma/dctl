#pragma once
#include <boost/range/begin.hpp>        // begin
#include <boost/range/concepts.hpp>     // BOOST_CONCEPT_ASSERT, SinglePassRangeConcept
#include <boost/range/end.hpp>          // end
#include <boost/range/iterator.hpp>     // range_iterator

namespace dctl {
namespace egdb {
namespace detail {

template<class Range>
class RankRemove
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));

private:
        using InputIterator = typename boost::range_iterator<Range>::type;
        mutable int delta{};
        mutable InputIterator first;
        InputIterator last;

public:
        explicit RankRemove(Range const& src) : first{begin(src)}, last{end(src)} {}

        template<class T>
        auto operator()(T value) const
        {
                while (first != last && *first < value) { ++first; ++delta; }
                return value - delta;
        }
};

}       // namespace detail
}       // namespace egdb
}       // namespace dctl
