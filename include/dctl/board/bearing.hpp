#pragma once
#include <dctl/board/angle.hpp> // _deg, rotate
#include <dctl/color_piece.hpp> // white

namespace dctl {
namespace board {

template<class Board, class color, class Reverse>
constexpr auto bearing_v = rotate(Board::orientation, ((color{} == white_c) ^ Reverse{}) ? 0_deg : 180_deg);

}       // namespace board
}       // namespace dctl
