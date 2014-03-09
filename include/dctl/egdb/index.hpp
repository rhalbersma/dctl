#pragma once
#include <dctl/board/mask.hpp>
#include <dctl/egdb/binomial.hpp>               // Binomial
#include <dctl/utility/make_const_callable.hpp> // make_const_callable
#include <boost/range/adaptor/reversed.hpp>     // reversed
#include <boost/range/adaptor/transformed.hpp>  // transformed
#include <boost/range/algorithm/find_if.hpp>    // find_if
#include <boost/range/begin.hpp>                // begin
#include <boost/range/concepts.hpp>             // BOOST_CONCEPT_ASSERT, SinglePassRangeConcept
#include <boost/range/end.hpp>                  // end
#include <boost/range/irange.hpp>               // irange
#include <boost/range/numeric.hpp>              // accumulate
#include <cassert>                              // assert
#include <tuple>
#include <utility>                              // pair

namespace dctl {
namespace egdb {

// input: an index in the range [0, choose(N, K))
// output: a strictly decreasing range { sq_K, ..., sq_1 }
// with all elements in the range [0, N)
template<class Index, class Binomial = BinomialTable<>>
auto colex_combination_unrank(std::pair<int /* N */, int /* K */> const& combination, Index index, Binomial const& = Binomial{})
{
        assert(0 <= combination.second && combination.second <= combination.first);
        assert(0 <= index && index < Binomial::coefficient(combination.first, combination.second));
        return boost::irange(1, combination.second + 1)
                | boost::adaptors::reversed
                | boost::adaptors::transformed(util::make_const_callable(
                        [index = index, sq = combination.first](auto i) mutable {
                        sq = *boost::find_if(boost::irange(0, sq) | boost::adaptors::reversed, [&](auto n) {
                                return Binomial::coefficient(n, i) <= index;
                        });
                        index -= Binomial::coefficient(sq, i);
                        return sq;
                }))
        ;
}

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i + delta_i, with delta_i = #(pat_i < dst_i)
template<class Range>
auto rank_inserted(Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return boost::adaptors::transformed(util::make_const_callable(
                [delta = 0, first = begin(pat), last = end(pat)](auto value) mutable {
                while (first != last && *first <= value + delta) { ++first; ++delta; }
                return value + delta;
        }));
}

// input: a strictly increasing pattern range { pat_1, ..., pat_P }
// output: applied to a non-overlapping strictly increasing source range { src_1, ..., src_K },
// a strictly increasing destination range { dst_1, ..., dst_K },
// where dst_i = src_i - delta_i, with delta_i = #(pat_i < src_i)
template<class Range>
auto rank_removed(Range const& pat)
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        return boost::adaptors::transformed(util::make_const_callable(
                [delta = 0, first = begin(pat), last = end(pat)](auto value) mutable {
                while (first != last && *first < value) { ++first; ++delta; }
                return value - delta;
        }));
}

// input: a strictly increasing range { sq_1, ..., sq_K }
// with all elements in the range [0, N)
// output: an index in the range [0, choose(N, K))
template<class Range, class Binomial = BinomialTable<>>
auto colex_combination_rank(Range const& src, Binomial const& = Binomial{})
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        using Index = typename Binomial::value_type;
        return boost::accumulate(
                src, Index{0}, [i = 1](auto index, auto sq) mutable {
                return index + Binomial::coefficient(sq, i++);
        });
}

}       // namespace egdb
}       // namespace dctl
