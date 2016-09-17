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

template<Color Value>
using color_constant = std::integral_constant<Color, Value>;

using black_type = color_constant<Color::black>;
using white_type = color_constant<Color::white>;

constexpr auto operator!(Color const c) noexcept
{
        return static_cast<Color>(!xstd::to_underlying_type(c));
}

template<class Color>
using opposite = color_constant<!Color::value>;

}       // namespace dctl
