#pragma once
#include <dctl/egdb/binomial.hpp>               // Binomial
#include <dctl/utility/make_const_callable.hpp> // make_const_callable
#include <boost/range/adaptor/reversed.hpp>     // reversed
#include <boost/range/adaptor/transformed.hpp>  // transformed
#include <boost/range/algorithm/find_if.hpp>    // find_if
#include <boost/range/concepts.hpp>             // BOOST_CONCEPT_ASSERT, SinglePassRangeConcept
#include <boost/range/irange.hpp>               // irange
#include <boost/range/numeric.hpp>              // accumulate
#include <cassert>                              // assert
#include <utility>                              // pair

namespace dctl {
namespace egdb {
namespace colex {

// input: an index in the range [0, choose(N, K))
// output: a strictly increasing range { sq_1, ..., sq_K }
// with all elements in the range [0, N)
template<class Index, class Binomial = BinomialTable<>>
auto combination_unrank(std::pair<int /* N */, int /* K */> const& combination, Index index, Binomial const& = Binomial{})
{
        assert(0 <= combination.second && combination.second <= combination.first);
        assert(0 <= index && index < Binomial::coefficient(combination.first, combination.second));
        using Range = typename Binomial::range_type;
        return Range
        {
                  boost::irange(1, combination.second + 1)
                | boost::adaptors::reversed
                | boost::adaptors::transformed(util::make_const_callable(
                        [index = index, sq = combination.first](auto i) mutable {
                        sq = *boost::find_if(boost::irange(0, sq) | boost::adaptors::reversed, [&](auto n) {
                                return Binomial::coefficient(n, i) <= index;
                        });
                        index -= Binomial::coefficient(sq, i);
                        return sq;
                }))
        };
}

// input: a strictly increasing range { sq_1, ..., sq_K }
// with all elements in the range [0, N)
// output: an index in the range [0, choose(N, K))
template<class Range, class Binomial = BinomialTable<>>
auto combination_rank(Range const& src, Binomial const& = Binomial{})
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        using Index = typename Binomial::index_type;
        return boost::accumulate(
                src, Index{0}, [i = 1](auto index, auto sq) mutable {
                return index + Binomial::coefficient(sq, i++);
        });
}

}       // namespace colex
}       // namespace egdb
}       // namespace dctl
