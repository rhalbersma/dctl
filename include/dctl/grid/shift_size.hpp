#pragma once
#include <dctl/angle/degrees.hpp>       // D00, D045, D090, D135, D180, D225, D270, D315

namespace dctl {
namespace grid {

template<class Grid>
constexpr auto shift_size(int direction) noexcept
{
        switch(angle::make_angle(direction)) {
        case angle::D000: return Grid::right     ;
        case angle::D045: return Grid::right_up  ;
        case angle::D090: return Grid::up        ;
        case angle::D135: return Grid::left_up   ;
        case angle::D180: return Grid::left      ;
        case angle::D225: return Grid::left_down ;
        case angle::D270: return Grid::down      ;
        case angle::D315: return Grid::right_down;
        default: return throw "Error", direction;
        }
}

}       // namespace grid
}       // namespace dctl
