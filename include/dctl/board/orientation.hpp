#pragma once
#include <dctl/angle.hpp>               // _deg, inverse, rotate
#include <dctl/position/color.hpp>      // white

namespace dctl {

template<class Board, bool Color>
constexpr auto orientation_v = rotate(inverse(Board::orientation), Color == Color::white ? 0_deg : 180_deg);

}       // namespace dctl
