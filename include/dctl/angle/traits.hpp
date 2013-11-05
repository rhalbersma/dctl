#pragma once
#include <dctl/angle/angle.hpp>         // Angle
#include <dctl/angle/degrees.hpp>       // _deg

namespace dctl {

/*

        The standard enumeration of angles

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

inline
constexpr auto is_orthogonal(Angle const& alpha) noexcept
{
        return alpha % 90_deg == 0_deg;
}

inline
constexpr auto is_diagonal(Angle const& alpha) noexcept
{
        return alpha % 90_deg == 45_deg;
}

inline
constexpr auto is_up(Angle const& alpha) noexcept
{
        return 0_deg < alpha && alpha < 180_deg;
}

inline
constexpr auto is_down(Angle const& alpha) noexcept
{
        return 180_deg < alpha;
}

inline
constexpr auto is_left(Angle const& alpha) noexcept
{
        return 90_deg < alpha && alpha < 270_deg;
}

inline
constexpr auto is_right(Angle const& alpha) noexcept
{
        return 270_deg < alpha || (0_deg <= alpha && alpha < 90_deg);
}

inline
constexpr auto is_positive(Angle const& alpha) noexcept
{
        return 0_deg < alpha && alpha <= 180_deg;
}

inline
constexpr auto is_negative(Angle const& alpha) noexcept
{
        return alpha == 0_deg || 180_deg < alpha;
}

}       // namespace dctl
