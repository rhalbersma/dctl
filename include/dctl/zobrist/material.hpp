#pragma once
#include <array>                        // array
#include <cstdint>                      // uint64_t
#include <dctl/random/uniform.hpp>      // runif, cat_zero_runif

namespace dctl {
namespace random {

template<int N>
struct Incremental
{
        static std::array<uint64_t, N> const pieces[];
        static std::array<uint64_t, N> const kings;
        static std::array<uint64_t, 2> const to_move;
};

template<int N>
std::array<uint64_t, N> const
Incremental<N>::pieces[] =
{
        runif<N>(),
        runif<N>()
};

template<int N>
std::array<uint64_t, N> const
Incremental<N>::kings = runif<N>();

template<int N>
std::array<uint64_t, 2> const
Incremental<N>::to_move = cat_zero_runif<1>();

}       // namespace random
}       // namespace dctl
