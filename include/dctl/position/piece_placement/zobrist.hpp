#pragma once
#include <array>                        // array
#include <cstdint>                      // uint64_t
#include <dctl/random/uniform.hpp>      // runif

namespace dctl {
namespace random {

template<int N>
struct PiecePlacement
{
        static std::array<uint64_t, N> const pieces[];
        static std::array<uint64_t, N> const kings;
};

template<int N>
std::array<uint64_t, N> const
PiecePlacement<N>::pieces[] =
{
        runif<N>(),
        runif<N>()
};

template<int N>
std::array<uint64_t, N> const
PiecePlacement<N>::kings = runif<N>();

}       // namespace random
}       // namespace dctl
