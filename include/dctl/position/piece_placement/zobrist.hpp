#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/random/uniform.hpp>      // runif
#include <array>                        // array
#include <cstddef>                      // size_t
#include <cstdint>                      // uint64_t

namespace dctl {
namespace zobrist {

template<std::size_t N>
class PiecePlacement
{
        static std::array<uint64_t, N> const by_color[2];
        static std::array<uint64_t, N> const by_piece[2];
public:
        static auto const& pieces(Color c)
        {
                return by_color[static_cast<std::size_t>(c)];
        }

        static auto const& pieces(Piece p)
        {
                return by_piece[static_cast<std::size_t>(p)];
        }
};

template<std::size_t N>
std::array<uint64_t, N> const
PiecePlacement<N>::by_color[2] =
{
        random::runif<N>(),
        random::runif<N>()
};

template<std::size_t N>
std::array<uint64_t, N> const
PiecePlacement<N>::by_piece[2] =
{
        random::runif<N>(),
        random::runif<N>()
};

}       // namespace zobrist
}       // namespace dctl
