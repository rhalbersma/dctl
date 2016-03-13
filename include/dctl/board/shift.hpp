#pragma once
#include <dctl/board/angle.hpp> // angle
#include <dctl/board/grid.hpp>  // EdgedGrid
#include <xstd/cstddef.hpp>     // _z
#include <stdexcept>            // invalid_argument

namespace dctl {
namespace board {

struct Shift
{
private:
        constexpr auto left_down () const noexcept { return (grid.width() + grid.edge()) / 2; }
        constexpr auto right_down() const noexcept { return left_down() + 1;                }
        constexpr auto right     () const noexcept { return right_down() - left_down();     }
        constexpr auto down      () const noexcept { return right_down() + left_down();     }
        constexpr auto left_up   () const noexcept { return right_down();                   }
        constexpr auto right_up  () const noexcept { return left_down();                    }
        constexpr auto left      () const noexcept { return right();                        }
        constexpr auto up        () const noexcept { return down();                         }
public:
        OuterGrid grid;

        constexpr auto operator()(angle const a) const
        {
                using namespace xstd::support_literals;
                switch (a.degrees()) {
                case   0 : return right()     ;
                case  45 : return right_up()  ;
                case  90 : return up()        ;
                case 135 : return left_up()   ;
                case 180 : return left()      ;
                case 225 : return left_down() ;
                case 270 : return down()      ;
                case 315 : return right_down();
                default  : return throw std::invalid_argument("Shift direction angles shall be a multiple of 45 degrees"), 0;
                }
        }
};

}       // namespace board
}       // namespace dctl
