#pragma once
#include <dctl/zobrist/side.hpp>

namespace dctl {

struct Side
{
        enum: bool {
                black = false,
                red = black,
                white = true,
                pass = true
        };
};

inline
auto zobrist_hash(bool to_move)
{
        using Zobrist = zobrist::Side;
        using Index = typename Zobrist::index_type;

        return Zobrist::to_move[to_move];
}

}       // namespace dctl
