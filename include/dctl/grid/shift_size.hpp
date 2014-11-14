#pragma once
#include <dctl/angle.hpp>               // Angle
#include <stdexcept>                    // invalid_argument

namespace dctl {
namespace grid {
namespace detail {

template<class Grid> constexpr auto left_down(Grid const& g) { return g.left_down(); }
template<class Grid> constexpr auto right_down(Grid const& g) { return left_down(g) + 1; }

        // orthogonal directions

template<class Grid> constexpr auto right(Grid const& g) { return right_down(g) - left_down(g); }
template<class Grid> constexpr auto down(Grid const& g) { return right_down(g) + left_down(g); }

        // equivalent directions

template<class Grid> constexpr auto left_up(Grid const& g) { return right_down(g); }
template<class Grid> constexpr auto right_up(Grid const& g) { return left_down(g); }
template<class Grid> constexpr auto left(Grid const& g) { return right(g); }
template<class Grid> constexpr auto up(Grid const& g) { return down(g); }

}       // namespace detail

template<class Grid>
constexpr auto shift_size(Grid const& g, Angle const& alpha)
{
        switch (alpha) {
        case   0: return detail::right(g)     ;
        case  45: return detail::right_up(g)  ;
        case  90: return detail::up(g)        ;
        case 135: return detail::left_up(g)   ;
        case 180: return detail::left(g)      ;
        case 225: return detail::left_down(g) ;
        case 270: return detail::down(g)      ;
        case 315: return detail::right_down(g);
        default: return throw std::invalid_argument("Shift direction angles shall be a multiple of 45 degrees"), 0;
        }
}

}       // namespace grid
}       // namespace dctl
