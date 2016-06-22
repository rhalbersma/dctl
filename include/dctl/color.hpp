#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type

namespace dctl {

enum class color
:
        unsigned char
{
        black = 0,
        white = 1
};

constexpr auto operator!(color const c) noexcept
{
        return static_cast<color>(!xstd::to_underlying_type(c));
}

}       // namespace dctl
