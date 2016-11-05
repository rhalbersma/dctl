#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <type_traits>          // integral_constant

namespace dctl {

enum class color : unsigned char { black, white };

constexpr auto operator!(color const c) noexcept
{
        return static_cast<color>(!xstd::to_underlying_type(c));
}

template<color Side>
using color_ = std::integral_constant<color, Side>;

template<class color>
using opposite = color_<!color::value>;

using black_ = color_<color::black>;
using white_ = color_<color::white>;

template<color Side>
constexpr auto color_c = color_<Side>{};

template<color Side>
constexpr auto operator!(color_<Side>) noexcept
{
        return color_c<!Side>;
}

inline constexpr auto black_c = color_c<color::black>;
inline constexpr auto white_c = color_c<color::white>;

enum class piece : unsigned char { pawn, king };

template<piece Type>
using piece_ = std::integral_constant<piece, Type>;

using pawn_ = piece_<piece::pawn>;
using king_ = piece_<piece::king>;

template<piece Type>
constexpr auto piece_c = piece_<Type>{};

inline constexpr auto pawn_c = piece_c<piece::pawn>;
inline constexpr auto king_c = piece_c<piece::king>;

struct  all_ {};
struct none_ {};

inline constexpr auto  all_c =  all_{};
inline constexpr auto none_c = none_{};

}       // namespace dctl
