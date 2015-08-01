#pragma once
#include <dctl/board/angle/angle.hpp>         // Angle
#include <dctl/board/angle/literals.hpp>      // _deg

namespace dctl {

/*

                 90
                 |
            135  |  45
               \ | /
                \|/
        180 ----- ----- 0
                /|\
               / | \
            225  |  315
                 |
                270

*/

constexpr auto is_orthogonal(Angle const& a) noexcept
{
        return a % 90_deg == 0_deg;
}

constexpr auto is_diagonal(Angle const& a) noexcept
{
        return a % 90_deg == 45_deg;
}

constexpr auto is_up(Angle const& a) noexcept
{
        return 0_deg < a && a < 180_deg;
}

constexpr auto is_down(Angle const& a) noexcept
{
        return 180_deg < a;
}

constexpr auto is_left(Angle const& a) noexcept
{
        return 90_deg < a && a < 270_deg;
}

constexpr auto is_right(Angle const& a) noexcept
{
        return 270_deg < a || (0_deg <= a && a < 90_deg);
}

constexpr auto is_positive(Angle const& a) noexcept
{
        return 0_deg < a && a <= 180_deg;
}

constexpr auto is_negative(Angle const& a) noexcept
{
        return a == 0_deg || 180_deg < a;
}

}       // namespace dctl
