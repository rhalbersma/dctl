#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/detail/set_filter.hpp>        // set_filter
#include <dctl/core/board/initial.hpp>                  // initial
#include <dctl/core/board/file_rank.hpp>                // file, rank
#include <dctl/core/board/rectangular.hpp>              // rectangular
#include <dctl/core/state/color_piece.hpp>              // opposite
#include <algorithm>                                    // min, max

namespace dctl::core {

template<class Board>
constexpr auto is_placeable = std::min(Board::width, Board::height) >= 1 && (not Board::is_inverted || std::max(Board::width, Board::height) > 1);

template<class Board>
constexpr auto is_pushable = std::min(Board::width, Board::height) >= 2;

template<class Board>
constexpr auto is_jumpable = std::min(Board::width, Board::height) >= 3 && (not Board::is_inverted || std::max(Board::width, Board::height) > 3);

template<class Board>
struct invert;

template<int Width, int Height, bool IsInverted, bool IsOrthogonalJump>
struct invert<rectangular<Width, Height, IsInverted, IsOrthogonalJump>>
:
        rectangular<Width, Height, not IsInverted, IsOrthogonalJump>
{};

template<class Board>
using invert_t = typename invert<Board>::type;

template<class Board>
struct remove_orthogonal_captures;

template<int Width, int Height, bool IsInverted, bool IsOrthogonalJump>
struct remove_orthogonal_captures<rectangular<Width, Height, IsInverted, IsOrthogonalJump>>
:
        rectangular<Width, Height, IsInverted, false>
{};

template<class Board>
using remove_orthogonal_captures_t = typename remove_orthogonal_captures<Board>::type;

template<class Board>
struct add_orthogonal_captures;

template<int Width, int Height, bool IsInverted, bool IsOrthogonalJump>
struct add_orthogonal_captures<rectangular<Width, Height, IsInverted, IsOrthogonalJump>>
:
        rectangular<Width, Height, IsInverted, true>
{};

template<class Board>
using add_orthogonal_captures_t = typename add_orthogonal_captures<Board>::type;

template<class Board>
constexpr file<Board> file_v{};

template<class Board>
constexpr rank<Board> rank_v{};

template<class Board, class Color>
constexpr auto promotion_v = rank_v<Board>(not Color{}, 0);

template<class Board>
constexpr auto squares_v = detail::set_filter<Board>([](auto const sq) {
        return sq < Board::size();
});

template<class Board>
constexpr auto initial_v = initial<Board>{};

}       // namespace dctl::core
