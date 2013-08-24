#pragma once
#include <dctl/angle/degrees.hpp>
#include <dctl/grid/coordinates.hpp>
#include <dctl/grid/coordinates/transform.hpp>

namespace dctl {
namespace board {
namespace detail {

template<class DestGrid, class FromSquare, class Angle>
constexpr auto transform(FromSquare const& from_sq, Angle theta) noexcept
{
        return grid::coordtosq<DestGrid>(grid::rotate(grid::sqtocoord(from_sq), theta));
}

}       // namespace detail

template<class Board>
constexpr auto square_to_bit(int N) noexcept
{
        return detail::transform<typename Board::InternalGrid>(grid::Square<typename Board::ExternalGrid>{N}, Board::orientation);
}

template<class Board>
constexpr auto bit_to_square(int N) noexcept
{
        return detail::transform<typename Board::ExternalGrid>(grid::Square<typename Board::InternalGrid>{N}, angle::inverse(Board::orientation));
}

}       // namespace board
}       // namespace dctl
