#pragma once
#include <dctl/angle/compass.hpp>       // Compass
#include <dctl/angle/degrees.hpp>       // D000, D180
#include <dctl/angle/transform.hpp>     // inverse, rotate

namespace dctl {
namespace board {

template<class Board, bool Color>
using Compass = angle::Compass<
        angle::rotate(
                angle::inverse(Board::orientation),
                Color ? 0_deg : 180_deg
        )
>;

}       // namespace board
}       // namespace dctl
