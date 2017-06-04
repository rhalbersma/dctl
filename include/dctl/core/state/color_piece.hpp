#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <type_traits>          // integral_constant

namespace dctl {
namespace core {

template <class T, class... Args>
constexpr auto is_any_of_v = (std::is_same_v<T, Args> || ...);

enum class color : unsigned char { black, white };

template<color Side>
using color_ = std::integral_constant<color, Side>;

using black_ = color_<color::black>;
using white_ = color_<color::white>;

template<class ColorT>
constexpr auto is_color_v = is_any_of_v<ColorT, color, black_, white_>;

template<color Side>
constexpr auto color_c = color_<Side>{};

constexpr auto black_c = color_c<color::black>;
constexpr auto white_c = color_c<color::white>;

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

template<class PieceT>
constexpr auto is_piece_v = is_any_of_v<PieceT, piece, pawns_, kings_>;

template<piece Type>
constexpr auto piece_c = piece_<Type>{};

constexpr auto pawns_c = piece_c<piece::pawns>;
constexpr auto kings_c = piece_c<piece::kings>;

struct occup_ {};
struct empty_ {};

constexpr auto occup_c = occup_{};
constexpr auto empty_c = empty_{};

struct nullmove_t {};
constexpr auto nullmove = nullmove_t{};

}       // namespace core
}       // namespace dctl
