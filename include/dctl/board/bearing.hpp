#pragma once
#include <dctl/board/angle.hpp> // _deg, inverse, rotate
#include <dctl/color_piece.hpp>       // white

namespace dctl {
namespace board {

template<class Board, class Color, class Reverse>
constexpr auto bearing_v = rotate(Board::orientation, ((Color{} == white_type{}) ^ Reverse{}) ? 0_deg : 180_deg);

}       // namespace board
}       // namespace dctl
