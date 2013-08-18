#pragma once
#include <dctl/board/coordinates.hpp>
#include <dctl/board/coordinates/transform.hpp>
#include <dctl/angle/degrees.hpp>

namespace dctl {
namespace board {
namespace detail {

template<class FromGrid, class DestGrid, int Angle, int N>
struct transform
:
        Coordinates2Square<
                DestGrid,
                Coordinates<
                        mpl::lazy::rotate<
                                Square2Coordinates< Square<FromGrid, N> >,
                                angle::Degrees< Angle >
                        >::type::row,
                        mpl::lazy::rotate<
                                Square2Coordinates< Square<FromGrid, N> >,
                                angle::Degrees< Angle >
                        >::type::col
                >
        >
{};

}       // namespace detail

template<class Board, int N>
struct square_to_bit
:
        detail::transform< typename
                Board::ExternalGrid, typename
                Board::InternalGrid,
                Board::orientation,
                N
        >
{};

template<class Board, int N>
struct bit_to_square
:
        detail::transform< typename
                Board::InternalGrid, typename
                Board::ExternalGrid,
                angle::inverse(Board::orientation),
                N
        >
{};

}       // namespace board
}       // namespace dctl

