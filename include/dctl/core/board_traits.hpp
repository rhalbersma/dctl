#pragma once
#include <dctl/core/board/mask/detail/copy_if.hpp>
#include <dctl/core/board/mask/column.hpp>   // column
#include <dctl/core/board/mask/initial.hpp>  // initial
#include <dctl/core/board/mask/row.hpp>      // row
#include <dctl/core/board/rectangular.hpp>   // rectangular
#include <dctl/core/color_piece.hpp>         // opposite
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
constexpr board::mask::column<Board, Color> column_v{};

template<class Board, class Color>
constexpr board::mask::row<Board, Color> row_v{};

template<class Board, class Color>
constexpr auto promotion_v = row_v<Board, opposite<Color>>(0);

template<class Board>
constexpr auto squares_v = board::mask::detail::copy_if<Board>([](auto const sq) {
        return sq < Board::size();
});

template<class Board, class Color>
constexpr auto initial_v = board::mask::initial<Board, Color>{};

}       // namespace dctl
