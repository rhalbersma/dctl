#pragma once
#include <boost/range/numeric.hpp>      // accumulate
#include <array>                        // array
#include <cstddef>                      // size_t

namespace dctl {
namespace zobrist {

template<class T, std::size_t N, class SinglePassRange>
auto hash_xor_accumulate(std::array<T, N> const& arr, SinglePassRange const& rng)
{
        return boost::accumulate(rng, T{0}, [&arr](auto const& hash, auto const& elem) {
                return hash ^ arr[elem];
        });
}

}       // namespace zobrist
}       // namespace dctl