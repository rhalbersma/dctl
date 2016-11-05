#pragma once
#include <dctl/board/mask/row.hpp>    // row
#include <dctl/color_piece.hpp>       // opposite

namespace dctl {
namespace board {
namespace mask {

template<class Board, class color>
constexpr auto promotion_v = row<Board, opposite<color>>{}(0);

}       // namespace mask
}       // namespace board
}       // namespace dctl
