#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/utility/random/uniform.hpp>      // runif
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <array>                        // array
#include <cstddef>                      // size_t

namespace dctl {
namespace zobrist {

template<std::size_t N>
class base_state
{
        static std::array<std::size_t, N> const by_Color[2];
        static std::array<std::size_t, N> const by_Piece[2];
public:
        static auto const& pieces(Color c)
        {
                return by_Color[xstd::to_underlying_type(c)];
        }

        static auto const& pieces(Piece p)
        {
                return by_Piece[xstd::to_underlying_type(p)];
        }
};

template<std::size_t N>
std::array<std::size_t, N> const
base_state<N>::by_Color[2] =
{
        random::runif<N>(),
        random::runif<N>()
};

template<std::size_t N>
std::array<std::size_t, N> const
base_state<N>::by_Piece[2] =
{
        random::runif<N>(),
        random::runif<N>()
};

}       // namespace zobrist
}       // namespace dctl
