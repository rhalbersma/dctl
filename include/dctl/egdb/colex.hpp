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
#include <dctl/bit/detail/intrinsic.hpp>

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
        auto const dst = Range
        {
                  boost::irange(1, combination.second + 1)
                | boost::adaptors::reversed
                | boost::adaptors::transformed(util::make_const_callable(
                        [index = index, N = combination.first](auto k) mutable {
                        auto const sq_k = N = *boost::find_if(boost::irange(0, N) | boost::adaptors::reversed, [&](auto n) {
                                return Binomial::coefficient(n, k) <= index;
                        });
                        index -= Binomial::coefficient(sq_k, k);
                        return sq_k;
                }))
        };
        assert(0 <= *begin(dst) && *rbegin(dst) < combination.first);
        return dst;
}

// input: a strictly increasing range { sq_1, ..., sq_K }
// with all elements in the range [0, N)
// output: an index in the range [0, choose(N, K))
template<class Range, class Binomial = BinomialTable<>>
auto combination_rank(Range const& src, Binomial const& = Binomial{})
{
        BOOST_CONCEPT_ASSERT(( boost::SinglePassRangeConcept<Range> ));
        using Index = typename Binomial::index_type;
        auto const index = boost::accumulate(
                src, Index{0}, [k = 1](auto result, auto sq_k) mutable {
                return result + Binomial::coefficient(sq_k, k++);
        });
        assert(0 <= index && index < Binomial::coefficient(*rbegin(src) + 1, src.count()));
        return index;
}

template<class Index, class Binomial = BinomialTable<>>
auto combination_unrank0(std::pair<int /* N */, int /* K */> const& combination, Index index, Binomial const& = Binomial{})
{
        using Range = typename Binomial::range_type;
        Range dst{0};
        int sq_k = combination.first;
        for (int k = combination.second; k > 0; --k) {
                while (Binomial::coefficient(sq_k, k) > index)
                        --sq_k;
                index -= Binomial::coefficient(sq_k, k);
                dst |= 1ULL << sq_k;
        }
        return dst;
}

template<class Range, class Binomial = BinomialTable<>>
auto combination_rank0(Range src, Binomial const& = Binomial{})
{
        using Index = typename Binomial::index_type;
        Index index{0};
        auto k = 1;
        while (src) {
                auto sq_k = bit::intrinsic::bsfnz(src);
                index += Binomial::coefficient(sq_k, k++);
                src ^= 1ULL << sq_k;
        }
        return index;
}

template<class BB>
BB free_square_bitmap(int logical_square, BB const& occupied)
{
        BB const behind = (1ULL << logical_square) - 1;
        int skipped = bit::intrinsic::popcount(occupied & behind);
        BB empty_ahead = ~(occupied | behind);
        while (skipped--)
                empty_ahead &= empty_ahead - 1;
        return 1ULL << bit::intrinsic::bsfnz(empty_ahead);
}

template<class Index, class Range, class Binomial = BinomialTable<>>
auto combination_unrank1(std::pair<int /* N */, int /* K */> const& combination, Index index, Range const& pat, Binomial const& = Binomial{})
{
        //using Range = typename Binomial::range_type;
        Range dst{0};
        int sq_k = combination.first;
        for (int k = combination.second; k > 0; --k) {
                while (Binomial::coefficient(sq_k, k) > index)
                        --sq_k;
                index -= Binomial::coefficient(sq_k, k);
                dst |= free_square_bitmap(sq_k, pat);
        }
        return dst;
}

template<class Range, class Binomial = BinomialTable<>>
auto combination_rank1(Range src, Range const& pat, Binomial const& = Binomial{})
{
        using Index = typename Binomial::index_type;
        Index index{0};
        auto k = 1;
        while (src) {
                auto const sq_k = bit::intrinsic::bsfnz(src);
                src ^= 1ULL << sq_k;
                auto const skipped = bit::intrinsic::popcount(pat & ((1ULL << sq_k) - 1));
                index += Binomial::coefficient(sq_k - skipped, k++);
        }
        return index;
}

}       // namespace colex
}       // namespace egdb
}       // namespace dctl
