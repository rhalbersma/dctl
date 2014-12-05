#pragma once

namespace dctl {

enum class Color
:
        bool
{
        black = false,
        white = true
};

inline
constexpr auto operator!(Color c) noexcept
{
        return static_cast<Color>(!static_cast<bool>(c));
}

inline
constexpr auto flip(Color& c) noexcept
{
        c = !c;
}

}       // namespace dctl
