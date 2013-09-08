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
                Color? angle::D000 : angle::D180
        )
>;

}       // namespace board
}       // namespace dctl
