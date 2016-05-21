#pragma once
#include <algorithm>    // min, max

namespace dctl {
namespace board {

template<class Board>
constexpr auto is_placeable = std::min(Board::width, Board::height) >= 1 && (!Board::is_inverted || std::max(Board::width, Board::height) > 1);

template<class Board>
constexpr auto is_pushable = std::min(Board::width, Board::height) >= 2;

template<class Board>
constexpr auto is_jumpable = std::min(Board::width, Board::height) >= 3 && (!Board::is_inverted || std::max(Board::width, Board::height) > 3);

template<class Board>
struct invert
:
        rectangular<Board::width, Board::height, !Board::is_inverted, Board::is_orthogonal_captures>
{};

template<class T>
using invert_t = typename invert<T>::type;

template<class Board>
struct remove_orthogonal_captures
:
        rectangular<Board::width, Board::height, Board::is_inverted, false>
{};

template<class T>
using remove_orthogonal_captures_t = typename remove_orthogonal_captures<T>::type;

template<class Board>
struct add_orthogonal_captures
:
        rectangular<Board::width, Board::height, Board::is_inverted, true>
{};

template<class T>
using add_orthogonal_captures_t = typename add_orthogonal_captures<T>::type;

}       // namespace board
}       // namespace dctl
