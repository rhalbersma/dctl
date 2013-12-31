#pragma once
#include <cstdint>                      // uint64_t
#include <dctl/utility/make_array.hpp>  // array, make_array
#include <dctl/zobrist/generate.hpp>    // zero_generate

namespace dctl {
namespace zobrist {

template<int M, int N>
struct Restricted
{
        using index_type = uint64_t;

        static std::array<index_type, 1 + N> const index[];
        static std::array<index_type, 1 + M> const moves[];
};

template<int M, int N>
std::array<typename Restricted<M, N>::index_type, 1 + N> const
Restricted<M, N>::index[] =
{
        make_array<1 + N>(zero_generate),
        make_array<1 + N>(zero_generate)
};

template<int M, int N>
std::array<typename Restricted<M, N>::index_type, 1 + M> const
Restricted<M, N>::moves[] =
{
        make_array<1 + M>(zero_generate),
        make_array<1 + M>(zero_generate)
};

}       // namespace zobrist
}       // namespace dctl
