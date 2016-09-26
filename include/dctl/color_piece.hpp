#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <type_traits>          // integral_constant

namespace dctl {

enum class Color
:
        unsigned char
{
        black = 0,
        white = 1
};

template<Color Side>
using color_constant = std::integral_constant<Color, Side>;

using black_type = color_constant<Color::black>;
using white_type = color_constant<Color::white>;

constexpr auto operator!(Color const c) noexcept
{
        return static_cast<Color>(!xstd::to_underlying_type(c));
}

template<class Color>
using opposite = color_constant<!Color::value>;

enum class Piece
:
        unsigned char
{
        pawn = 0,
        king = 1
};

template<Piece Type>
using piece_constant = std::integral_constant<Piece, Type>;

using pawn_type = piece_constant<Piece::pawn>;
using king_type = piece_constant<Piece::king>;

}       // namespace dctl
