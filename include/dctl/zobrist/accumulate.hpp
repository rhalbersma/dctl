#pragma once
#include <boost/range/numeric.hpp>      // accumulate
#include <array>                        // array
#include <cassert>                      // assert
#include <cstddef>                      // size_t

namespace dctl {
namespace zobrist {

template<class SinglePassRange, class T, std::size_t N>
auto accumulate(SinglePassRange const& rng, std::array<T, N> const& arr)
{
        return boost::accumulate(
                rng, T{0}, [&](auto const& hash, auto const& elem) {
                return hash ^ arr[static_cast<std::size_t>(elem)];
        });
}

}       // namespace zobrist
}       // namespace dctl
