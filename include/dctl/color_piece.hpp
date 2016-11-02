#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <type_traits>          // integral_constant

namespace dctl {

enum class Color : unsigned char { black, white };

constexpr auto operator!(Color const c) noexcept
{
        return static_cast<Color>(!xstd::to_underlying_type(c));
}

template<Color Side>
using color_constant = std::integral_constant<Color, Side>;

template<class Color>
using opposite = color_constant<!Color::value>;

using black_ = color_constant<Color::black>;
using white_ = color_constant<Color::white>;

template<Color Side>
constexpr auto color_c = color_constant<Side>{};

template<Color Side>
constexpr auto operator!(color_constant<Side>) noexcept
{
        return color_c<!Side>;
}

inline constexpr auto black_c = color_c<Color::black>;
inline constexpr auto white_c = color_c<Color::white>;

enum class Piece : unsigned char { pawn, king };

template<Piece Type>
using piece_constant = std::integral_constant<Piece, Type>;

using pawn_ = piece_constant<Piece::pawn>;
using king_ = piece_constant<Piece::king>;

template<Piece Type>
constexpr auto piece_c = piece_constant<Type>{};

inline constexpr auto pawn_c = piece_c<Piece::pawn>;
inline constexpr auto king_c = piece_c<Piece::king>;

struct  all_ {};
struct none_ {};

inline constexpr auto  all_c =  all_{};
inline constexpr auto none_c = none_{};

}       // namespace dctl
