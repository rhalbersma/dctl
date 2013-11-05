#pragma once
#include <dctl/angle/degrees.hpp>       // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/transform.hpp>     // inverse, rotate

namespace dctl {
namespace angle {

/*

        The standard point of view

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

template<int Direction>
struct Compass
{
        static constexpr auto orientation = Angle{Direction};
        static constexpr auto right       = rotate(  0_deg, orientation);
        static constexpr auto right_up    = rotate( 45_deg, orientation);
        static constexpr auto up          = rotate( 90_deg, orientation);
        static constexpr auto left_up     = rotate(135_deg, orientation);
        static constexpr auto left        = rotate(180_deg, orientation);
        static constexpr auto left_down   = rotate(225_deg, orientation);
        static constexpr auto down        = rotate(270_deg, orientation);
        static constexpr auto right_down  = rotate(315_deg, orientation);
};

}       // namespace angle
}       // namespace dctl
