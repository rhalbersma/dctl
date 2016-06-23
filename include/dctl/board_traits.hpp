#pragma once
#include <dctl/board/rectangular.hpp>   // rectangular
#include <xstd/type_traits.hpp>         // _t
#include <algorithm>                    // min, max
#include <cstddef>                      // size_t

namespace dctl {

template<class Board>
constexpr auto is_placeable = std::min(Board::width, Board::height) >= 1 && (!Board::is_inverted || std::max(Board::width, Board::height) > 1);

template<class Board>
constexpr auto is_pushable = std::min(Board::width, Board::height) >= 2;

template<class Board>
constexpr auto is_jumpable = std::min(Board::width, Board::height) >= 3 && (!Board::is_inverted || std::max(Board::width, Board::height) > 3);

namespace detail {

template<class Board>
struct invert;

template<std::size_t Width, std::size_t Height, bool IsInverted, bool IsOrthogonalCaptures>
struct invert<board::rectangular<Width, Height, IsInverted, IsOrthogonalCaptures>>
:
        board::rectangular<Width, Height, !IsInverted, IsOrthogonalCaptures>
{};

template<class Board>
struct remove_orthogonal_captures;

template<std::size_t Width, std::size_t Height, bool IsInverted, bool IsOrthogonalCaptures>
struct remove_orthogonal_captures<board::rectangular<Width, Height, IsInverted, IsOrthogonalCaptures>>
:
        board::rectangular<Width, Height, IsInverted, false>
{};

template<class Board>
struct add_orthogonal_captures;

template<std::size_t Width, std::size_t Height, bool IsInverted, bool IsOrthogonalCaptures>
struct add_orthogonal_captures<board::rectangular<Width, Height, IsInverted, IsOrthogonalCaptures>>
:
        board::rectangular<Width, Height, IsInverted, true>
{};

}       // namespace detail

template<class Board>
using invert_t = typename detail::invert<xstd::_t<Board>>::type;

template<class Board>
using remove_orthogonal_captures_t = typename detail::remove_orthogonal_captures<xstd::_t<Board>>::type;

template<class Board>
using add_orthogonal_captures_t = typename detail::add_orthogonal_captures<xstd::_t<Board>>::type;

}       // namespace dctl
