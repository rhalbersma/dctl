#pragma once
#include <algorithm>                    // accumulate
#include <array>                        // array
#include <cassert>                      // assert
#include <cstddef>                      // size_t
#include <iterator>                     // begin, end

namespace dctl {
namespace zobrist {

template<class Container, class T, std::size_t N>
auto accumulate(Container const& cont, std::array<T, N> const& arr)
{
        using std::begin; using std::end;
        return std::accumulate(
                begin(cont), end(cont), T{0},
                [&](auto const& hash, auto const& elem) {
                return hash ^ arr[static_cast<std::size_t>(elem)];
        });
}

}       // namespace zobrist
}       // namespace dctl
