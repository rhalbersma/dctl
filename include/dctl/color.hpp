#pragma once
#include <cstddef>      // size_t

namespace dctl {

enum class Color
:
        std::size_t
{
        black = 0,
        white = 1
};

inline
constexpr auto operator!(Color c) noexcept
{
        return static_cast<Color>(!static_cast<std::size_t>(c));
}

inline
constexpr auto flip(Color& c) noexcept
{
        c = !c;
}

}       // namespace dctl
