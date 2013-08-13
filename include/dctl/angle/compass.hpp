#pragma once
#include <dctl/angle/degrees.hpp>       // D000, D045, D090, D135, D180, D225, D270, D315

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
        static constexpr auto A = -Board::orientation + (Color? D000::value : D180::value);

public:
        static constexpr auto right      = make_angle(D000::value + A);
        static constexpr auto right_up   = make_angle(D045::value + A);
        static constexpr auto up         = make_angle(D090::value + A);
        static constexpr auto left_up    = make_angle(D135::value + A);
        static constexpr auto left       = make_angle(D180::value + A);
        static constexpr auto left_down  = make_angle(D225::value + A);
        static constexpr auto down       = make_angle(D270::value + A);
        static constexpr auto right_down = make_angle(D315::value + A);
};

}       // namespace angle
}       // namespace dctl
