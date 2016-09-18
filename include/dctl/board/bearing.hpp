#pragma once
#include <dctl/board/angle.hpp> // _deg, inverse, rotate
#include <dctl/color.hpp>       // white

namespace dctl {

template<class Board, class Color, class Reverse>
constexpr auto bearing_v = rotate(Board::orientation, ((Color{} == white_type{}) ^ Reverse{}) ? 0_deg : 180_deg);

}       // namespace dctl
