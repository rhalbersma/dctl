#pragma once
#include <algorithm>                            // accumulate
#include <cassert>                              // assert
#include <iterator>
#include <utility>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/numeric.hpp>
#include <dctl/bit/detail/intrinsic.hpp>        // count
#include <dctl/egdb/binomial.hpp>
#include <dctl/bit/set.hpp>
#include <iostream>

namespace dctl {
namespace egdb {

template<class OutputRange, class T, class Generator = Binomial<384, 192>>
auto colex_combination_unrank(T index, std::pair<int, int> const& combination, Generator const& = Generator{})
{
        OutputRange dst{};
        boost::transform(
                boost::irange(1, combination.second + 1) | boost::adaptors::reversed,
                std::inserter(dst, begin(dst)), [&, sq = combination.first](auto i) mutable {
                sq = *find_if(boost::irange(0, sq) | boost::adaptors::reversed, [&](auto n) {
                        return Generator::coefficient(n, i) <= index;
                });
                index -= Generator::coefficient(sq, i);
                return sq;
        });
        return dst;
}

template<class ForwardIterator>
struct MixIn
{
        mutable int delta{};
        mutable ForwardIterator first2;
        ForwardIterator last2;

        MixIn(ForwardIterator b, ForwardIterator e)
        :
                first2{b},
                last2{e}
        {}

        template<class T>
        auto operator()(T elem) const
        {
                static_assert(std::is_convertible<T, int>::value, "");
                while (first2 != last2 && *first2 <= elem + delta) {
                        ++first2;
                        ++delta;
                }
                return elem + delta;
        }
};

// https://svn.boost.org/trac/boost/ticket/4969
template<class ForwardIterator>
struct MixOut
{
        mutable int delta{};
        mutable ForwardIterator first2;
        ForwardIterator last2;

        MixOut(ForwardIterator b, ForwardIterator e)
        :
                first2{b},
                last2{e}
        {}

        template<class T>
        auto operator()(T elem) const
        {
                static_assert(std::is_convertible<T, int>::value, "");
                while (first2 != last2 && *first2 < elem) {
                        ++first2;
                        ++delta;
                }
                return elem - delta;
        }
};

template<class SinglePassRange1, class SinglePassRange2>
auto mixin(SinglePassRange1 const& src, SinglePassRange2 pattern)
{
        using It = decltype(begin(pattern));
        return src | boost::adaptors::transformed(MixIn<It>{begin(pattern), end(pattern)});
}

template<class SinglePassRange1, class SinglePassRange2>
auto mixout(SinglePassRange1 const& src, SinglePassRange2 pattern)
{
        using It = decltype(begin(pattern));
        return src | boost::adaptors::transformed(MixOut<It>{begin(pattern), end(pattern)});
}

template<class SinglePassRange, class Generator = Binomial<384, 192>>
auto colex_combination_rank(SinglePassRange const& rng, Generator const& = Generator{})
{
        return boost::accumulate(
                rng, typename Generator::value_type{0}, [i = 1](auto index, auto sq) mutable {
                return index + Generator::coefficient(sq, i++);
        });
}

//-------------------------------------------//

template<class OutputRange, class T, class Generator = Binomial<384, 192>>
auto colex_combination_unrank2(T index, std::pair<int, int> const& combination, Generator const& = Generator{})
{
        auto sq = combination.first;
        auto i = combination.second;
        OutputRange dst{};
        while (i) {
                while (Generator::coefficient(sq, i) > index)
                        --sq;
                index -= Generator::coefficient(sq, i--);
                dst.set(sq);
        }
        return dst;
}

template<class OutputRange, class SinglePassRange1, class SinglePassRange2>
auto mixin2(SinglePassRange1 src, SinglePassRange2 pattern)
{
        OutputRange dst{};
        while (!src.empty()) {
                auto sq = src.front();
                sq += bit::intrinsic::popcount(pattern.data() & ((1ULL << sq) - 1));
                dst.set(sq);
                src.pop_front();
        }
        return dst;
}

template<class SinglePassRange, class Generator = Binomial<384, 192>>
auto colex_combination_rank2(SinglePassRange rng, Generator const& = Generator{})
{
        auto index = std::ptrdiff_t{0}; auto i = 1;
        while (!rng.empty()) {
                auto sq = rng.front();
                index += Generator::coefficient(sq, i++);
                rng.pop_front();
        }
        return index;
}

template<class SinglePassRange1, class SinglePassRange2, class Generator = Binomial<384, 192>>
auto colex_combination_rank2_mixout(SinglePassRange1 rng, SinglePassRange2 pattern, Generator const& = Generator{})
{
        auto index = std::ptrdiff_t{0}; auto i = 1;
        while (!rng.empty()) {
                auto sq = rng.front();
                sq -= bit::intrinsic::popcount(pattern.data() & ((1ULL << sq) - 1));
                index += Generator::coefficient(sq, i++);
                rng.pop_front();
        }
        return index;
}

}       // namespace egdb
}       // namespace dctl
