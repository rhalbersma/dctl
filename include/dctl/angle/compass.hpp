#pragma once
#include <dctl/angle/degrees.hpp>       // D000, D045, D090, D135, D180, D225, D270, D315, rotate, inverse

namespace dctl {
namespace angle {

/*

     +++ PASSIVE COLOR POINT OF VIEW +++

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

      +++ ACTIVE COLOR POINT OF VIEW +++

*/

template
<
        bool Color,
        class Board
>
struct Compass
{
private:
        static constexpr auto theta = rotate(inverse(Board::orientation), (Color? D000 : D180));

public:
        static constexpr auto right      = rotate(D000, theta);
        static constexpr auto right_up   = rotate(D045, theta);
        static constexpr auto up         = rotate(D090, theta);
        static constexpr auto left_up    = rotate(D135, theta);
        static constexpr auto left       = rotate(D180, theta);
        static constexpr auto left_down  = rotate(D225, theta);
        static constexpr auto down       = rotate(D270, theta);
        static constexpr auto right_down = rotate(D315, theta);
};

}       // namespace angle
}       // namespace dctl
