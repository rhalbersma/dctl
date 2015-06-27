#pragma once
#include <xstd/type_traits.hpp> // to_underlying_type

namespace dctl {

enum class Player
:
        unsigned char
{
        black = 0,
        white = 1
};

constexpr auto operator!(Player p) noexcept
{
        return static_cast<Player>(!xstd::to_underlying_type(p));
}

}       // namespace dctl
