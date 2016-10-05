#pragma once
#include <dctl/board/mask/row.hpp>    // row
#include <dctl/color_piece.hpp>       // opposite

namespace dctl {
namespace board {
namespace mask {

template<class Board, class Color>
constexpr auto promotion_v = row<Board, opposite<Color>>{}(0);

}       // namespace mask
}       // namespace board
}       // namespace dctl
