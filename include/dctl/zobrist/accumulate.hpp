#pragma once
#include <array>        // array
#include <cstddef>      // size_t
#include <numeric>      // accumulate

namespace dctl {
namespace zobrist {

template<class SinglePassRange, class T, std::size_t N>
auto hash_xor_accumulate(SinglePassRange const& rng, std::array<T, N> const& arr)
{
        return std::accumulate(begin(rng), end(rng), T{0}, [&arr](auto const& hash, auto const& elem) {
                return hash ^ arr[elem];
        });
}

}       // namespace zobrist
}       // namespace dctl
