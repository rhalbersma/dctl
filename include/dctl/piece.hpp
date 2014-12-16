#pragma once
#include <cstddef>
#include <type_traits>

namespace dctl {

enum class Piece
:
        std::size_t
{
        pawn = 0,
        king = 1
};

template<Piece Type>
using PieceType = std::integral_constant<Piece, Type>;

using PiecePawnType = PieceType<Piece::pawn>;
using PieceKingType = PieceType<Piece::king>;

template<Piece Type>
constexpr auto PieceType_v = PieceType<Type>::value;

}       // namespace dctl
