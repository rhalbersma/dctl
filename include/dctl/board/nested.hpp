#pragma once

namespace dctl {
namespace board {

template<class Board, int Direction>
using jump_start = typename Board::template jump_start<Direction>::type;

}       // namespace board
}       // namespace dctl
