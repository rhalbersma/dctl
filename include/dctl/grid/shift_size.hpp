#pragma once
#include <stdexcept>                    // invalid_argument
#include <dctl/angle/angle.hpp>         // make_angle
#include <dctl/angle/degrees.hpp>       // D00, D045, D090, D135, D180, D225, D270, D315

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

template<class Grid, class T>
constexpr auto shift_size(T const& direction)
{
        switch (dctl::make_angle(direction)) {
        case angle::D000: return detail::ShiftSize<Grid>::right     ;
        case angle::D045: return detail::ShiftSize<Grid>::right_up  ;
        case angle::D090: return detail::ShiftSize<Grid>::up        ;
        case angle::D135: return detail::ShiftSize<Grid>::left_up   ;
        case angle::D180: return detail::ShiftSize<Grid>::left      ;
        case angle::D225: return detail::ShiftSize<Grid>::left_down ;
        case angle::D270: return detail::ShiftSize<Grid>::down      ;
        case angle::D315: return detail::ShiftSize<Grid>::right_down;
        default: return throw std::invalid_argument("Shift direction angles shall be a multiple of 45 degrees"), -1;
        }
}

}       // namespace grid
}       // namespace dctl
