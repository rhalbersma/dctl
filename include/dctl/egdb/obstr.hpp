#pragma once
#include <dctl/egdb/binomial.hpp>               // Binomial
#include <dctl/utility/make_const_callable.hpp> // make_const_callable
#include <boost/range/adaptor/transformed.hpp>  // transformed
#include <boost/range/begin.hpp>                // begin
#include <boost/range/concepts.hpp>             // BOOST_CONCEPT_ASSERT, SinglePassRangeConcept
#include <boost/range/end.hpp>                  // end

namespace dctl {
namespace egdb {
namespace obstr {

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i + delta_i, with delta_i = #(pat_i < dst_i)
template<class Range>
auto inserted(Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return boost::adaptors::transformed(util::make_const_callable(
                [delta = 0, first = boost::begin(pat), last = boost::end(pat)](auto value) mutable {
                while (first != last && *first <= value + delta) { ++first; ++delta; }
                return value + delta;
        }));
}

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a non-overlapping strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i - delta_i, with delta_i = #(pat_i < src_i)
template<class Range>
auto removed(Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return boost::adaptors::transformed(util::make_const_callable(
                [delta = 0, first = boost::begin(pat), last = boost::end(pat)](auto value) mutable {
                while (first != last && *first < value) { ++first; ++delta; }
                return value - delta;
        }));
}

}       // namespace obstr
}       // namespace egdb
}       // namespace dctl
