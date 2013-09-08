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

template<int Orientation>
struct Compass
{
        static constexpr auto right      = rotate(D000, Orientation);
        static constexpr auto right_up   = rotate(D045, Orientation);
        static constexpr auto up         = rotate(D090, Orientation);
        static constexpr auto left_up    = rotate(D135, Orientation);
        static constexpr auto left       = rotate(D180, Orientation);
        static constexpr auto left_down  = rotate(D225, Orientation);
        static constexpr auto down       = rotate(D270, Orientation);
        static constexpr auto right_down = rotate(D315, Orientation);
};

}       // namespace angle
}       // namespace dctl
