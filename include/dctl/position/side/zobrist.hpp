#pragma once
#include <array>                        // array
#include <cstdint>                      // uint64_t
#include <dctl/random/uniform.hpp>      // zero_generate

namespace dctl {
namespace random {

struct Side
{
        static std::array<uint64_t, 2> const to_move;
};

std::array<uint64_t, 2> const
Side::to_move = cat_zero_runif<1>();

}       // namespace random

inline
auto zobrist_hash(bool to_move)
{
        using Zobrist = random::Side;
        return Zobrist::to_move[to_move];
}

}       // namespace dctl
