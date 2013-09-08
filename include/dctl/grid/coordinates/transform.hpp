#pragma once
#include <stdexcept>                    // logic_error
#include <dctl/angle/angle.hpp>         // make_angle
#include <dctl/angle/degrees.hpp>       // D000, L090, R090, D180
#include <dctl/grid/coordinates.hpp>    // Coordinates

namespace dctl {
namespace grid {

template<class T>
constexpr auto rotate(Coordinates const& coord, T const& theta)
{
        switch(dctl::make_angle(theta)) {
        case angle::D000: return coord;
        case angle::L090: return Coordinates{  coord.col(), -coord.row() };
        case angle::R090: return Coordinates{ -coord.col(),  coord.row() };
        case angle::D180: return Coordinates{ -coord.row(), -coord.col() };
        default: return throw std::logic_error("Coordinates rotation angles shall be a multiple of 90 degrees."), coord;
        }
}

}       // namespace grid
}       // namespace dctl
