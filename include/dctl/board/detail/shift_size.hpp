#pragma once
#include <dctl/board/angle.hpp>         // angle
#include <dctl/board/detail/grid.hpp>   // OuterGrid
#include <xstd/cstddef.hpp>             // _zu
#include <stdexcept>                    // invalid_argument

namespace dctl {
namespace board {
namespace detail {

class shift_size
{
        constexpr auto left_down (OuterGrid const g) const noexcept { return (g.width() + g.edge()) / 2;   }
        constexpr auto right_down(OuterGrid const g) const noexcept { return left_down(g) + 1;             }
        constexpr auto right     (OuterGrid const g) const noexcept { return right_down(g) - left_down(g); }
        constexpr auto down      (OuterGrid const g) const noexcept { return right_down(g) + left_down(g); }
        constexpr auto left_up   (OuterGrid const g) const noexcept { return right_down(g);                }
        constexpr auto right_up  (OuterGrid const g) const noexcept { return left_down(g);                 }
        constexpr auto left      (OuterGrid const g) const noexcept { return right(g);                     }
        constexpr auto up        (OuterGrid const g) const noexcept { return down(g);                      }
public:
        constexpr auto operator()(OuterGrid const g, angle const a) const
        {
                using namespace xstd::support_literals;
                switch (a.degrees()) {
                case   0 : return right(g)     ;
                case  45 : return right_up(g)  ;
                case  90 : return up(g)        ;
                case 135 : return left_up(g)   ;
                case 180 : return left(g)      ;
                case 225 : return left_down(g) ;
                case 270 : return down(g)      ;
                case 315 : return right_down(g);
                default  : return static_cast<void>(throw std::invalid_argument("Not a multiple of 45 degrees")), std::size_t(0);
                }
        }
};

}       // namespace detail
}       // namespace board
}       // namespace dctl
