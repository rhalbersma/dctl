#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type
#include <cstddef>              // size_t

namespace dctl {

enum class Color
:
        unsigned char
{
        black = 0,
        white = 1
};

inline
constexpr auto operator!(Color c) noexcept
{
        return static_cast<Color>(!xstd::to_underlying_type(c));
}

inline
constexpr auto flip(Color& c) noexcept
{
        c = !c;
}

}       // namespace dctl
