#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <type_traits>          // integral_constant

namespace dctl {

enum class color : unsigned char { black, white };

template<color Side>
using color_ = std::integral_constant<color, Side>;

using black_ = color_<color::black>;
using white_ = color_<color::white>;

template<color Side>
constexpr auto color_c = color_<Side>{};

inline constexpr auto black_c = color_c<color::black>;
inline constexpr auto white_c = color_c<color::white>;

constexpr auto operator!(color const c) noexcept
{
        return static_cast<color>(!xstd::to_underlying_type(c));
}

template<class Color>
using opposite = color_<!Color::value>;

template<color Side>
constexpr auto operator!(color_<Side>) noexcept
{
        return color_c<!Side>;
}

enum class piece : unsigned char { pawns, kings };

template<piece Type>
using piece_ = std::integral_constant<piece, Type>;

using pawns_ = piece_<piece::pawns>;
using kings_ = piece_<piece::kings>;

template<piece Type>
constexpr auto piece_c = piece_<Type>{};

inline constexpr auto pawns_c = piece_c<piece::pawns>;
inline constexpr auto kings_c = piece_c<piece::kings>;

struct occup_ {};
struct empty_ {};

inline constexpr auto occup_c = occup_{};
inline constexpr auto empty_c = empty_{};

struct nullmove_t {};
inline constexpr auto nullmove = nullmove_t{};

}       // namespace dctl
