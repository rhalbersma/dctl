#pragma once
#include <dctl/board/angle.hpp> // Angle, _deg
#include <dctl/board/grid.hpp>  // Grid, rotate
#include <algorithm>            // min_element
#include <iterator>             // cbegin, cend

namespace dctl {
namespace board {

constexpr auto size_minimizing_orientation(OuterGrid const& g)
{
        // simulate a constexpr lambda (not allowed in C++14)
        struct grid_less
        {
                OuterGrid g;

                constexpr auto operator()(Angle lhs, Angle rhs) const noexcept
                {
                        return rotate(g, lhs).size() < rotate(g, rhs).size();
                }
        };

        constexpr Angle orientations[] = { 0_deg, 90_deg, 180_deg, 270_deg };
        return *std::min_element(std::cbegin(orientations), std::cend(orientations), grid_less{g});
}

}       // namespace board
}       // namespace dctl
