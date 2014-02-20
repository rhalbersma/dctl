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

namespace dctl {
namespace egdb {

template<class SinglePassRange, class Generator = Binomial<384, 192>>
auto colex_combination_rank(SinglePassRange const& rng, Generator const& = Generator{})
{
        return boost::accumulate(
                rng, typename Generator::value_type{0}, [i = 1](auto index, auto sq) mutable {
                return index + Generator::coefficient(sq, i++);
        });
}

template<class T, class Generator = Binomial<384, 192>>
auto colex_combination_unrank(T rank, std::pair<int, int> const& combination, Generator const& = Generator{})
{
        auto N = combination.first;
        auto K = combination.second;
        bit::Set<int, uint64_t, 1> rng;
        boost::transform(
                boost::irange(1, K + 1) | boost::adaptors::reversed,
                std::inserter(rng, begin(rng)), [&](auto k) {
                N = *find_if(boost::irange(0, N) | boost::adaptors::reversed, [=](auto n) {
                        return Generator::coefficient(n, k) <= rank;
                });
                rank -= Generator::coefficient(N, k);
                return N;
        });
        return rng;
}

}       // namespace egdb
}       // namespace dctl
