#pragma once
#include <type_traits>

namespace dctl {

enum class Piece
:
        unsigned
{
        pawn,
        king
};

template<Piece Type>
using PieceType = std::integral_constant<Piece, Type>;

using PiecePawnType = PieceType<Piece::pawn>;
using PieceKingType = PieceType<Piece::king>;

template<Piece Type>
constexpr auto PieceType_v = PieceType<Type>::value;

}       // namespace dctl
