#pragma once
#include <type_traits>	// integral_constant

namespace dctl {

enum class Piece
:
        unsigned char
{
        pawn = 0,
        king = 1
};

template<Piece Value>
using piece_constant = std::integral_constant<Piece, Value>;

using pawn_type = piece_constant<Piece::pawn>;
using king_type = piece_constant<Piece::king>;

}       // namespace dctl
