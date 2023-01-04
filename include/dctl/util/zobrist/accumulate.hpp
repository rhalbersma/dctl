#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <array>        // array
#include <cstddef>      // size_t
#include <numeric>      // accumulate

namespace dctl {
namespace zobrist {

template<class T, std::size_t N, class SinglePassRange>
auto hash_xor_accumulate(std::array<T, N> const& arr, SinglePassRange const& rng)
{
        return std::accumulate(rng.cbegin(), rng.cend(), T{0}, [&arr](auto const& hash, auto const& elem) {
                return hash ^ arr[elem];
        });
}

}       // namespace zobrist
}       // namespace dctl
