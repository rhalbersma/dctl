#pragma once
#include <cstddef>      // size_t

namespace dctl {

enum class Piece
:
        std::size_t
{
        pawn = 0,
        king = 1
};

}       // namespace dctl
