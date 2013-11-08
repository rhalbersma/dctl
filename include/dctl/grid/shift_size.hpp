#pragma once
#include <stdexcept>                    // invalid_argument
#include <dctl/angle.hpp>               // Angle

namespace dctl {
namespace grid {
namespace detail {

template<class Grid>
struct ShiftSize
{
        static_assert(0 < Grid::edge_columns, "Shift size only uniform for grids with a positive number of edge columns");

        // diagonal directions

        static constexpr auto left_down = (Grid::width + Grid::edge_columns) / 2;
        static constexpr auto right_down = left_down + 1;

        // orthogonal directions

        static constexpr auto right = right_down - left_down;
        static constexpr auto down = right_down + left_down;

        // equivalent directions

        static constexpr auto left_up = right_down;
        static constexpr auto right_up = left_down;
        static constexpr auto left = right;
        static constexpr auto up = down;
};

}       // namespace detail

template<class T>
constexpr auto identity(T const& t) noexcept
{
        return t;
}

template<class Grid>
constexpr auto shift_size(Angle const& direction)
{
        switch (direction) {
        case   0: return detail::ShiftSize<Grid>::right     ;
        case  45: return detail::ShiftSize<Grid>::right_up  ;
        case  90: return detail::ShiftSize<Grid>::up        ;
        case 135: return detail::ShiftSize<Grid>::left_up   ;
        case 180: return detail::ShiftSize<Grid>::left      ;
        case 225: return detail::ShiftSize<Grid>::left_down ;
        case 270: return detail::ShiftSize<Grid>::down      ;
        case 315: return detail::ShiftSize<Grid>::right_down;
        default: return throw std::invalid_argument("Shift direction angles shall be a multiple of 45 degrees"), -1;
        }
}

}       // namespace grid
}       // namespace dctl
