#pragma once
#include <dctl/utility/random/uniform.hpp>      // cat_zero_runif
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <array>                        // array
#include <cstddef>                      // size_t

namespace dctl {
namespace zobrist {

template<std::size_t M, std::size_t N>
class MostRecentlyPushedKings
{
        static std::array<std::size_t, 1 + N> const index_[];
        static std::array<std::size_t, 1 + M> const count_[];
public:
        static auto const& index(Color c)
        {
                return index_[xstd::to_underlying_type(c)];
        }

        static auto const& count(Color c)
        {
                return count_[xstd::to_underlying_type(c)];
        }
};

template<std::size_t M, std::size_t N>
std::array<std::size_t, 1 + N> const
MostRecentlyPushedKings<M, N>::index_[] =
{
        random::cat_runif_zero<N>(),
        random::cat_runif_zero<N>()
};

template<std::size_t M, std::size_t N>
std::array<std::size_t, 1 + M> const
MostRecentlyPushedKings<M, N>::count_[] =
{
        random::cat_zero_runif<M>(),
        random::cat_zero_runif<M>()
};

}       // namespace zobrist
}       // namespace dctl
