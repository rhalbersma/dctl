#pragma once
#include <algorithm>                    // accumulate
#include <array>                        // array
#include <cstddef>                      // size_t
#include <iterator>                     // begin, end
#include <type_traits>                  // is_integral

namespace dctl {
namespace zobrist {

template<class Container, class T, std::size_t N>
auto accumulate(Container const& cont, std::array<T, N> const& arr)
{
        static_assert(std::is_integral<typename Container::value_type>::value, "");
        using std::begin; using std::end;
        return std::accumulate(
                begin(cont), end(cont), T{0},
                [&](auto hash, auto elem){
                assert(0 <= elem && elem < N);
                return hash ^ arr[static_cast<std::size_t>(elem)];
        });
}

}       // namespace zobrist
}       // namespace dctl
