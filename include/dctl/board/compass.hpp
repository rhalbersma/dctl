#pragma once
#include <dctl/angle.hpp>               // _deg, inverse, rotate
#include <dctl/position/color.hpp>           // white

namespace dctl {
namespace board {
namespace detail {

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

template<int Orientation>
struct Directions
{
        static constexpr auto right       = rotate(  0_deg, Orientation);
        static constexpr auto right_up    = rotate( 45_deg, Orientation);
        static constexpr auto up          = rotate( 90_deg, Orientation);
        static constexpr auto left_up     = rotate(135_deg, Orientation);
        static constexpr auto left        = rotate(180_deg, Orientation);
        static constexpr auto left_down   = rotate(225_deg, Orientation);
        static constexpr auto down        = rotate(270_deg, Orientation);
        static constexpr auto right_down  = rotate(315_deg, Orientation);
};

}       // namespace detail

template<class Board, bool Color>
using Compass = detail::Directions
<
        rotate(inverse(Board::orientation), Color == Color::white ? 0_deg : 180_deg)
>;

}       // namespace board
}       // namespace dctl
