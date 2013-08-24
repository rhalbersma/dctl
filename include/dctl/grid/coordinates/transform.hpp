#pragma once
#include <dctl/angle/degrees.hpp>       // D000, L090, R090, D180
#include <dctl/grid/coordinates.hpp>    // Coordinates

namespace dctl {
namespace grid {

constexpr auto rotate(Coordinates const& coord, int theta) noexcept
{
        switch(angle::make_angle(theta)) {
        case angle::D000: return coord;
        case angle::L090: return Coordinates{  coord.col(), -coord.row() };
        case angle::R090: return Coordinates{ -coord.col(),  coord.row() };
        case angle::D180: return Coordinates{ -coord.row(), -coord.col() };
        default: return throw "Error", coord;
        }
}

}       // namespace grid
}       // namespace dctl
