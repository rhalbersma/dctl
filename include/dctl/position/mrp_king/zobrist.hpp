#pragma once
#include <dctl/random/uniform.hpp>      // cat_zero_runif
#include <array>                        // array
#include <cstddef>                      // size_t
#include <cstdint>                      // uint64_t

namespace dctl {
namespace zobrist {

template<std::size_t M, std::size_t N>
struct MostRecentlyPushedKing
{
        static std::array<uint64_t, 1 + N> const index[];
        static std::array<uint64_t, 1 + M> const moves[];
};

template<std::size_t M, std::size_t N>
std::array<uint64_t, 1 + N> const
MostRecentlyPushedKing<M, N>::index[] =
{
        random::cat_zero_runif<N>(),
        random::cat_zero_runif<N>()
};

template<std::size_t M, std::size_t N>
std::array<uint64_t, 1 + M> const
MostRecentlyPushedKing<M, N>::moves[] =
{
        random::cat_zero_runif<M>(),
        random::cat_zero_runif<M>()
};

}       // namespace zobrist
}       // namespace dctl
