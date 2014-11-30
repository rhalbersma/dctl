#pragma once
#include <dctl/angle.hpp>               // _deg, inverse, rotate
#include <dctl/color.hpp>      // white

namespace dctl {

template<class Board, Color ToMove>
constexpr auto orientation_v = rotate(inverse(Board::orientation), ToMove == Color::white ? 0_deg : 180_deg);

}       // namespace dctl
