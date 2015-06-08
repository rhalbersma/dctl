#pragma once
#include <dctl/angle/angle.hpp>         // Angle, rotate
#include <dctl/angle/literals.hpp>      // _deg
#include <dctl/angle/transform.hpp>     // rotate

namespace dctl {

/*

                   up
                   |
          left_up  |  right_up
                 \ | /
                  \|/
         left ----- ----- right
                  /|\
                 / | \
        left_down  |  right_down
                   |
                  down

*/

constexpr auto right(Angle a = 0_deg) noexcept
{
        return rotate(0_deg, a);
}

constexpr auto right_up(Angle a = 0_deg) noexcept
{
        return rotate(45_deg, a);
}

constexpr auto up(Angle a = 0_deg) noexcept
{
        return rotate(90_deg, a);
}

constexpr auto left_up(Angle a = 0_deg) noexcept
{
        return rotate(135_deg, a);
}

constexpr auto left(Angle a = 0_deg) noexcept
{
        return rotate(180_deg, a);
}

constexpr auto left_down(Angle a = 0_deg) noexcept
{
        return rotate(225_deg, a);
}

constexpr auto down(Angle a = 0_deg) noexcept
{
        return rotate(270_deg, a);
}

constexpr auto right_down(Angle a = 0_deg) noexcept
{
        return rotate(315_deg, a);
}

}       // namespace dctl
