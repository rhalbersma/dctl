#pragma once
#include <cstddef>                              // size_t
#include <cstdint>                              // uint64_t
#include <dctl/position/mru_king/mru_king.hpp>  // MostRecentlyUsedKing
#include <dctl/random/uniform.hpp>              // cat_zero_runif

namespace dctl {
namespace random {

template<int M, int N>
struct MostRecentlyUsedKing
{
        static std::array<uint64_t, 1 + N> const index[];
        static std::array<uint64_t, 1 + M> const moves[];
};

template<int M, int N>
std::array<uint64_t, 1 + N> const
MostRecentlyUsedKing<M, N>::index[] =
{
        cat_zero_runif<N>(),
        cat_zero_runif<N>()
};

template<int M, int N>
std::array<uint64_t, 1 + M> const
MostRecentlyUsedKing<M, N>::moves[] =
{
        cat_zero_runif<M>(),
        cat_zero_runif<M>()
};

}       // namespace random
}       // namespace dctl
