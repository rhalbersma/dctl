#pragma once
#include <dctl/angle.hpp>               // Angle, _deg
#include <dctl/grid/dimensions.hpp>     // dimensions, rotate
#include <dctl/grid/grid.hpp>           // grid
#include <xstd/algorithm.hpp>           // min_element
#include <xstd/iterator.hpp>            // cbegin, cend

namespace dctl {
namespace grid {

template<int EdgeColumns>
struct grid_less
{
        Dimensions dim;

        constexpr auto rotated_grid_size(Angle const& theta) const
        {
                return Grid<EdgeColumns>{rotate(dim, theta)}.size();
        }

        constexpr auto operator()(Angle const& lhs, Angle const& rhs) const
        {
                return rotated_grid_size(lhs) < rotated_grid_size(rhs);
        }
};

template<int EdgeColumns>
constexpr auto size_minimizing_orientation(Dimensions const& dim)
{
        constexpr Angle orientations[] = { 0_deg, 90_deg, 180_deg, 270_deg };
        return Angle{*xstd::min_element(xstd::cbegin(orientations), xstd::cend(orientations), grid_less<EdgeColumns>{dim})};
}

}       // namespace grid
}       // namespace dctl
