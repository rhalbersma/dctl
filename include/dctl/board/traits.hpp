#pragma once

namespace dctl {
namespace board {

template<class Board>
constexpr auto is_empty = (Board::width == 0 || Board::height == 0) || (Board::width == 1 && Board::height == 1 && Board::is_inverted);

template<class Board>
constexpr auto is_moveable = Board::width >= 2 && Board::height >= 2;

template<class Board>
constexpr auto is_jumpable = Board::width >= 3 && Board::height >= 3;

}       // namespace board
}       // namespace dctl
