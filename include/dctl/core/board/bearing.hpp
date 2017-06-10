#pragma once
#include <dctl/core/board/angle.hpp>            // _deg, rotate
#include <dctl/core/state/color_piece.hpp>      // white

namespace dctl::core {

template<class Board, class Color, class Reverse>
constexpr auto bearing_v = rotate(Board::orientation, ((Color{} == white_c) ^ Reverse{}) ? 0_deg : 180_deg);

}       // namespace dctl::core
