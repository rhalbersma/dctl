#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>  // integral_constant
#include <utility>      // to_underlying

namespace dctl::core {

enum struct color
:
        unsigned char
{
        black,
        white,
        size
};

[[nodiscard]] constexpr auto operator!(color c) noexcept
{
        return static_cast<color>(!std::to_underlying(c));
}

template<color C>
using color_ = std::integral_constant<color, C>;

using black_ = color_<color::black>;
using white_ = color_<color::white>;

template<class Color>
using not_ = color_<!Color::value>;

template<color C>
inline constexpr auto color_c = color_<C>();

inline constexpr auto black_c = color_c<color::black>;
inline constexpr auto white_c = color_c<color::white>;

template<color C>
[[nodiscard]] constexpr auto operator!(color_<C>) noexcept
{
        return color_c<!C>;
}

}       // namespace dctl::core
