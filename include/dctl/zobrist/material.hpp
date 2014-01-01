#pragma once
#include <array>                        // array
#include <cstdint>                      // uint64_t
#include <dctl/random/uniform.hpp>      // runif

namespace dctl {
namespace random {

template<int N>
struct Material
{
        static std::array<uint64_t, N> const pieces[];
        static std::array<uint64_t, N> const kings;
};

template<int N>
std::array<uint64_t, N> const
Material<N>::pieces[] =
{
        runif<N>(),
        runif<N>()
};

template<int N>
std::array<uint64_t, N> const
Material<N>::kings = random::runif<N>();

}       // namespace random
}       // namespace dctl
