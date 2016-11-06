#pragma once
#include <dctl/board/rectangular.hpp>   // rectangular
#include <algorithm>                    // min, max
#include <cstddef>                      // size_t

namespace dctl {
namespace board {

template<class Board>
constexpr auto is_placeable = std::min(Board::width, Board::height) >= 1 && (!Board::is_inverted || std::max(Board::width, Board::height) > 1);

template<class Board>
constexpr auto is_pushable = std::min(Board::width, Board::height) >= 2;

template<class Board>
constexpr auto is_jumpable = std::min(Board::width, Board::height) >= 3 && (!Board::is_inverted || std::max(Board::width, Board::height) > 3);

template<class Board>
struct invert;

template<class Board>
using invert_t = typename invert<Board>::type;

template<std::size_t Width, std::size_t Height, bool IsInverted, bool IsOrthogonalJump>
struct invert<rectangular<Width, Height, IsInverted, IsOrthogonalJump>>
:
        rectangular<Width, Height, !IsInverted, IsOrthogonalJump>
{};

template<class Board>
struct remove_orthogonal_captures;

template<class Board>
using remove_orthogonal_captures_t = typename remove_orthogonal_captures<Board>::type;

template<std::size_t Width, std::size_t Height, bool IsInverted, bool IsOrthogonalJump>
struct remove_orthogonal_captures<rectangular<Width, Height, IsInverted, IsOrthogonalJump>>
:
        rectangular<Width, Height, IsInverted, false>
{};

template<class Board>
struct add_orthogonal_captures;

template<class Board>
using add_orthogonal_captures_t = typename add_orthogonal_captures<Board>::type;

template<std::size_t Width, std::size_t Height, bool IsInverted, bool IsOrthogonalJump>
struct add_orthogonal_captures<rectangular<Width, Height, IsInverted, IsOrthogonalJump>>
:
        rectangular<Width, Height, IsInverted, true>
{};

}       // namespace board
}       // namespace dctl
