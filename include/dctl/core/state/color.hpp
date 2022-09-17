#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <xstd/type_traits.hpp> // is_integral_constant_v
#include <xstd/utility.hpp>     // to_underlying
#include <type_traits>          // integral_constant, is_same_v

namespace dctl::core {

enum struct color
:
        unsigned char
{
        black,
        white,
        size
};

constexpr auto operator!(color c) noexcept
{
        return static_cast<color>(!xstd::to_underlying(c));
}

template<color N>
using color_ = std::integral_constant<color, N>;

using black_ = color_<color::black>;
using white_ = color_<color::white>;

template<class T>
using not_ = color_<!T::value>;

template<class T>
inline constexpr auto is_color = std::is_same_v<T, color> || xstd::is_integral_constant_v<T, color>;

template<color N>
inline constexpr auto color_c = color_<N>();

inline constexpr auto black_c = color_c<color::black>;
inline constexpr auto white_c = color_c<color::white>;

template<color N>
constexpr auto operator!(color_<N>) noexcept
{
        return color_c<!N>;
}

}       // namespace dctl::core
