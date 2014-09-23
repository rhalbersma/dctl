#pragma once
#include <dctl/angle/angle.hpp>         // Angle rotate
#include <dctl/angle/degrees.hpp>       // _deg
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

inline
constexpr auto right(Angle const& orientation = Angle{}) noexcept
{
        return rotate(0_deg, orientation);
}

inline
constexpr auto right_up(Angle const& orientation = Angle{}) noexcept
{
        return rotate(45_deg, orientation);
}

inline
constexpr auto up(Angle const& orientation = Angle{}) noexcept
{
        return rotate(90_deg, orientation);
}

inline
constexpr auto left_up(Angle const& orientation = Angle{}) noexcept
{
        return rotate(135_deg, orientation);
}

inline
constexpr auto left(Angle const& orientation = Angle{}) noexcept
{
        return rotate(180_deg, orientation);
}

inline
constexpr auto left_down(Angle const& orientation = Angle{}) noexcept
{
        return rotate(225_deg, orientation);
}

inline
constexpr auto down(Angle const& orientation = Angle{}) noexcept
{
        return rotate(270_deg, orientation);
}

inline
constexpr auto right_down(Angle const& orientation = Angle{}) noexcept
{
        return rotate(315_deg, orientation);
}

}       // namespace dctl
