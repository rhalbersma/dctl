#pragma once
#include <dctl/angle.hpp>       // _deg, inverse, rotate
#include <dctl/color.hpp>       // Color, white

namespace dctl {

template<class Board, Color ToMove, bool Reverse = false>
constexpr auto orientation_v = rotate(inverse(Board::orientation), ((ToMove == Color::white) ^ Reverse) ? 0_deg : 180_deg);

}       // namespace dctl
