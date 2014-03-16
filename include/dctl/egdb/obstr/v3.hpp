#pragma once
#include <dctl/utility/make_const_callable.hpp> // make_const_callable
#include <boost/range/adaptor/transformed.hpp>  // transformed
#include <boost/range/begin.hpp>                // begin
#include <boost/range/concepts.hpp>             // BOOST_CONCEPT_ASSERT, SinglePassRangeConcept
#include <boost/range/end.hpp>                  // end

namespace dctl {
namespace egdb {
namespace obstr {
namespace v3 {

template<class Range>
auto mirrored()
{
        return boost::adaptors::transformed([](auto value) {
                return Range{}.max_size() - 1 - value;
        });
}

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i + delta_i, with delta_i = #(pat_i < dst_i)
template<class Range>
auto front_inserted(Range const& src, Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return Range {
                src | boost::adaptors::transformed(util::make_const_callable(
                        [first = boost::begin(pat), last = boost::end(pat), skipped = 0](auto sq_i) mutable {
                        while (first != last && *first <= sq_i + skipped) { ++first; ++skipped; }
                        return sq_i + skipped;
                }))
        };
}

template<class Range>
auto back_inserted(Range const& src, Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return Range {
                src | mirrored<Range>() | boost::adaptors::transformed(util::make_const_callable(
                        [first = boost::rbegin(pat), last = boost::rend(pat), skipped = 0](auto sq_i) mutable {
                        while (first != last && *first >= sq_i - skipped) { ++first; ++skipped; }
                        return sq_i - skipped;
                }))
        };
}

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a non-overlapping strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i - delta_i, with delta_i = #(pat_i < src_i)
template<class Range>
auto front_erased(Range const& src, Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return Range {
                src | boost::adaptors::transformed(util::make_const_callable(
                        [first = boost::begin(pat), last = boost::end(pat), skipped = 0](auto sq_i) mutable {
                        while (first != last && *first < sq_i) { ++first; ++skipped; }
                        return sq_i - skipped;
                }))
        };
}

template<class Range>
auto back_erased(Range const& src, Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return Range {
                src | boost::adaptors::reversed | boost::adaptors::transformed(util::make_const_callable(
                        [first = boost::rbegin(pat), last = boost::rend(pat), skipped = 0](auto sq_i) mutable {
                        while (first != last && *first > sq_i) { ++first; ++skipped; }
                        return sq_i + skipped;
                })) | mirrored<Range>()
        };
}

}       // namespace v3
}       // namespace obstr
}       // namespace egdb
}       // namespace dctl
