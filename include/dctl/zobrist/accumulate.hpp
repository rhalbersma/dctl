#pragma once
#include <array>        // array
#include <cstddef>      // size_t
#include <numeric>      // accumulate

namespace dctl {
namespace zobrist {

template<class T, std::size_t N, class SinglePassRange>
auto hash_xor_accumulate(std::array<T, N> const& arr, SinglePassRange const& rng)
{
        return std::accumulate(begin(rng), end(rng), T{0}, [&arr](auto const& hash, auto const& elem) {
                return hash ^ arr[elem];
        });
}

}       // namespace zobrist
}       // namespace dctl
