#pragma once
#include <dctl/egdb/binomial.hpp>               // Binomial
#include <dctl/utility/make_const_callable.hpp> // make_const_callable
#include <boost/range/adaptor/transformed.hpp>  // transformed
#include <boost/range/begin.hpp>                // begin
#include <boost/range/concepts.hpp>             // BOOST_CONCEPT_ASSERT, SinglePassRangeConcept
#include <boost/range/end.hpp>                  // end
#include <dctl/bit/input_range.hpp>
namespace dctl {
namespace egdb {
namespace obstr {

template<class Range>
auto mirrored()
{
        return boost::adaptors::transformed([](auto value) { return Range{}.max_size() - 1 - value; });
}

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i + delta_i, with delta_i = #(pat_i < dst_i)
template<class Range>
auto front_inserted(Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return boost::adaptors::transformed(util::make_const_callable(
                [delta = 0, first = boost::begin(pat), size = pat.size()](auto value) mutable {
                while (delta < size && *first <= value + delta) { ++first; ++delta; }
                return value + delta;
        }));
}

template<class Range>
auto back_inserted(Range const& src, Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return src | boost::adaptors::transformed(util::make_const_callable(
                [delta = 0, first = boost::rbegin(pat), last = boost::rend(pat)](auto value) mutable {
                while (first != last && *first >= value - delta) { ++first; ++delta; }
                return value - delta;
        }));
}

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a non-overlapping strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i - delta_i, with delta_i = #(pat_i < src_i)
template<class Range>
auto front_erased(Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return boost::adaptors::transformed(util::make_const_callable(
                [delta = 0, first = boost::begin(pat), last = boost::end(pat)](auto value) mutable {
                while (first != last && *first < value) { ++first; ++delta; }
                return value - delta;
        }));
}

template<class Range1, class Range2>
auto back_erased(Range1 const& src, Range2 const& pat)
{
        //BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return src | boost::adaptors::transformed(util::make_const_callable(
                [delta = 0, first = boost::rbegin(pat), last = boost::rend(pat)](auto value) mutable {
                while (first != last && *first > value) { ++first; ++delta; }
                return value + delta;
        }));
}

template<class Range>
auto back_inserted0(Range const& src, Range const& pat)
{
        Range dst;
        auto rit = rbegin(~pat);
        auto sq_prev = 0;
        for (auto const& sq_i : src) {
                while (sq_prev != sq_i) {
                        ++rit;
                        ++sq_prev;
                }
                dst.set(*rit);
        }
        return dst;
}

template<class Range>
auto back_inserted1(Range const& src, Range const& pat)
{
        Range dst;
        auto rit = rbegin(pat);
        auto re = rend(pat);
        auto skipped = 0;
        for (auto const& sq_i : src) {
                while (rit != re && *rit >= (64 - 1 - sq_i) - skipped) {
                        ++rit;
                        ++skipped;
                }
                dst.set((64- 1 - sq_i) - skipped);
        }
        return dst;
}

template<class Range>
auto back_erased0(Range const& src, Range const& pat)
{
        Range dst;
        auto rit = rbegin(~pat);
        auto sq_i = 0;
        for (auto sit = rbegin(src), re = rend(src); sit != re; ++sit) {
                while (*rit != *sit) {
                        ++rit;
                        ++sq_i;
                }
                dst.set(sq_i);
        }
        return dst;
}

template<class Range>
auto back_erased1(Range const& src, Range const& pat)
{
        Range dst;
        auto rit = rbegin(pat);
        auto re = rend(pat);
        auto skipped = 0;
        for (auto sit = rbegin(src), e = rend(src); sit != e; ++sit) {
                while (rit != re && *rit > *sit) {
                        ++rit;
                        ++skipped;
                }
                dst.set(64 - 1 - (*sit + skipped));
        }
        return dst;
}

}       // namespace obstr
}       // namespace egdb
}       // namespace dctl
