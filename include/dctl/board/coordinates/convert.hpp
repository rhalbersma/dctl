#pragma once
#include <dctl/board/coordinates.hpp>
#include <dctl/board/coordinates/transform.hpp>
#include <dctl/angle/degrees.hpp>

namespace dctl {
namespace board {
namespace detail {

template<class FromSquare, class DestGrid, int Angle>
struct transform
:
        Coordinates2Square<
                DestGrid, typename
                mpl::lazy::rotate<
                        Square2Coordinates< FromSquare >,
                        angle::Degrees< Angle >
                >::type
        >
{};

template<class DestGrid, class FromGrid>
constexpr auto xtransform(xSquare<FromGrid> const& square, int theta)
{
        return coordtosq<DestGrid>(xrotate(sqtocoord(square), theta));
}

}       // namespace detail

template<class Board, int N>
struct square_to_bit
:
        detail::transform<
                Square<typename Board::ExternalGrid, N>, typename
                Board::InternalGrid,
                Board::orientation
        >
{};

template<class Board>
constexpr auto xsquare_to_bit(int N)
{
        return detail::xtransform<typename Board::InternalGrid>(xSquare<typename Board::ExternalGrid>{N}, Board::orientation);
}

template<class Board, int N>
struct bit_to_square
:
        detail::transform<
                Square<typename Board::InternalGrid, N>, typename
                Board::ExternalGrid,
                angle::inverse(Board::orientation)
        >
{};

template<class Board>
constexpr auto xbit_to_square(int N)
{
        return detail::xtransform<typename Board::ExternalGrid>(xSquare<typename Board::InternalGrid>{N}, Board::orientation);
}

}       // namespace board
}       // namespace dctl

