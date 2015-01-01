#pragma once
#include <dctl/random/uniform.hpp>      // cat_zero_runif
#include <array>                        // array
#include <cstddef>                      // size_t
#include <cstdint>                      // uint64_t

namespace dctl {
namespace zobrist {

template<std::size_t M, std::size_t N>
class MostRecentlyPushedKings
{
        static std::array<uint64_t, 1 + N> const index_[];
        static std::array<uint64_t, 1 + M> const moves_[];
public:
        static auto index(Color c)
        {
                return index_[static_cast<std::size_t>(c)];
        }

        static auto moves(Color c)
        {
                return moves_[static_cast<std::size_t>(c)];
        }
};

template<std::size_t M, std::size_t N>
std::array<uint64_t, 1 + N> const
MostRecentlyPushedKings<M, N>::index_[] =
{
        random::cat_zero_runif<N>(),
        random::cat_zero_runif<N>()
};

template<std::size_t M, std::size_t N>
std::array<uint64_t, 1 + M> const
MostRecentlyPushedKings<M, N>::moves_[] =
{
        random::cat_zero_runif<M>(),
        random::cat_zero_runif<M>()
};

}       // namespace zobrist
}       // namespace dctl
