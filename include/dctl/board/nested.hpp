#pragma once

namespace dctl {
namespace board {

template<class Board, int Direction>
using shift_size = typename Board::template shift_size<Direction>::type;

template<class Board, int Direction>
using jump_start = typename Board::template jump_start<Direction>::type;

}       // namespace board
}       // namespace dctl
