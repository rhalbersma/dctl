#pragma once
#include <dctl/board/mask/detail/copy_if.hpp>
#include <dctl/board/mask/column.hpp>   // column
#include <dctl/board/mask/initial.hpp>  // initial
#include <dctl/board/mask/row.hpp>      // row
#include <dctl/board/rectangular.hpp>   // rectangular
#include <dctl/color_piece.hpp>         // opposite
#include <algorithm>                    // min, max

namespace dctl {

template<class Board>
constexpr auto is_placeable = std::min(Board::width, Board::height) >= 1 && (!Board::is_inverted || std::max(Board::width, Board::height) > 1);

template<class Board>
constexpr auto is_pushable = std::min(Board::width, Board::height) >= 2;

template<class Board>
constexpr auto is_jumpable = std::min(Board::width, Board::height) >= 3 && (!Board::is_inverted || std::max(Board::width, Board::height) > 3);

template<class Board>
struct invert;

template<int Width, int Height, bool IsInverted, bool IsOrthogonalJump>
struct invert<board::rectangular<Width, Height, IsInverted, IsOrthogonalJump>>
:
        board::rectangular<Width, Height, !IsInverted, IsOrthogonalJump>
{};

template<class Board>
using invert_t = typename invert<Board>::type;

template<class Board>
struct remove_orthogonal_captures;

template<int Width, int Height, bool IsInverted, bool IsOrthogonalJump>
struct remove_orthogonal_captures<board::rectangular<Width, Height, IsInverted, IsOrthogonalJump>>
:
        board::rectangular<Width, Height, IsInverted, false>
{};

template<class Board>
using remove_orthogonal_captures_t = typename remove_orthogonal_captures<Board>::type;

template<class Board>
struct add_orthogonal_captures;

template<int Width, int Height, bool IsInverted, bool IsOrthogonalJump>
struct add_orthogonal_captures<board::rectangular<Width, Height, IsInverted, IsOrthogonalJump>>
:
        board::rectangular<Width, Height, IsInverted, true>
{};

template<class Board>
using add_orthogonal_captures_t = typename add_orthogonal_captures<Board>::type;

template<class Board, class Color>
constexpr auto column_v = board::mask::column<Board, Color>{};

template<class Board, class Color>
constexpr auto row_v = board::mask::row<Board, Color>{};

template<class Board, class Color>
constexpr auto promotion_v = row_v<Board, opposite<Color>>(0);

template<class Board>
constexpr auto squares_v = board::mask::detail::copy_if<Board>([](auto const sq) {
        return sq < Board::size();
});

template<class Board, class Color>
constexpr auto initial_v = board::mask::initial<Board, Color>{};

}       // namespace dctl
