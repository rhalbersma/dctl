#pragma once
#include <dctl/angle.hpp>                               // Angle
#include <dctl/grid/coordinates/llo.hpp>                // Coordinates with Lower Left Origin
#include <dctl/grid/coordinates/sco.hpp>                // Coordinates with Screen Centered Origin
#include <dctl/grid/coordinates/ulo.hpp>                // Coordinates with Upper Left Origin
#include <dctl/grid/coordinates/square.hpp>             // Square with Upper Left Origin
#include <dctl/grid/coordinates/xy.hpp>                 // get_x, get_y
#include <dctl/grid/coordinates/detail/transform.hpp>   // swap_llo_ulo, sco_from_ulo, ulo_from_sco
#include <stdexcept>                                    // invalid_argument

namespace dctl {
namespace grid {
namespace sco {

// rotations among Coordinates with Screen Centered Origin

inline
constexpr auto rotate(Coordinates const& coord, Angle const& theta)
{
        switch (theta) {
        case   0: return coord;
        case  90: return Coordinates{ -get_y(coord),  get_x(coord) };
        case 180: return Coordinates{ -get_x(coord), -get_y(coord) };
        case 270: return Coordinates{  get_y(coord), -get_x(coord) };
        default: return throw std::invalid_argument("Coordinates rotation angles shall be a multiple of 90 degrees."), coord;
        }
}

}       // namespace sco

// conversions between Coordinates with Screen Centered Origin and Upper Left Origin

template<class Grid>
constexpr sco::Coordinates sco_from_ulo(ulo::Coordinates<Grid> const& coord)
{
        return
        {
                detail::sco_from_ulo(get_x(coord), Grid::width ),
                detail::sco_from_ulo(get_y(coord), Grid::height)
        };
}

template<class Grid>
constexpr ulo::Coordinates<Grid> ulo_from_sco(sco::Coordinates const& coord)
{
        return
        {
                detail::ulo_from_sco(get_x(coord), Grid::width ),
                detail::ulo_from_sco(get_y(coord), Grid::height)
        };
}

// conversions between Coordinates with Lower Left Origin and Upper Left Origin

template<class Grid>
constexpr llo::Coordinates<Grid> llo_from_ulo(ulo::Coordinates<Grid> const& coord)
{
        return { get_x(coord), detail::swap_llo_ulo(get_y(coord), Grid::height) };
}

template<class Grid>
constexpr ulo::Coordinates<Grid> ulo_from_llo(llo::Coordinates<Grid> const& coord)
{
        return { get_x(coord), detail::swap_llo_ulo(get_y(coord), Grid::height) };
}

namespace ulo {

// conversions between Squares and Coordinates with Upper Left Origin

template<class Grid>
constexpr Coordinates<Grid> ulo_from_sq(Square<Grid> const& square) noexcept
{
        // number of row pairs
        auto const Q = square.value() / Grid::modulo;

        // left edge of the zeroth row
        auto const R0 = square.value() % Grid::modulo;

        // left edge of the first row
        auto const R1 = R0 - Grid::edge_lo;

        // R0 is in the zeroth or first row
        auto const P = R1 >= 0;

        // squares from the left edge
        auto const R = P ? R1 : R0;

        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        auto const X = 2 * R + (P ^ !Grid::ul_parity);

        // 2x the row pairs + the row parity
        auto const Y = 2 * Q + P;

        return { X, Y };
}

template<class Grid>
constexpr auto sq_from_ulo(Coordinates<Grid> const& coord) noexcept
{
        // row parity
        auto const P = get_y(coord) % 2;

        // number of row pairs
        auto const Q = get_y(coord) / 2;

        // the left edge
        auto const L = P ? Grid::edge_lo : Grid::edge_le;

        // number of column pairs
        auto const S = get_x(coord) / 2;

        // squares from the left edge
        auto const R = (L + S) % Grid::modulo;

        auto const NUM = Grid::modulo * Q + R;

        return Square<Grid>{ NUM };
}

// conversion between Squares of relatively rotated Grids

template<class DestGrid, class FromGrid>
constexpr auto square_from_square(Square<FromGrid> const& from_sq, Angle const& theta)
{
        // perform a 5-step coordinate transformation
        // ulo::Square -> ulo::Coordinates -> sco::Coordinates -> ulo::Coordinates -> ulo::Square
        //                                    ^^^^^^^^^^^^^^^^
        //                                    *rotations here*
        return sq_from_ulo(
                grid::ulo_from_sco<DestGrid>(
                        rotate(
                                grid::sco_from_ulo(ulo_from_sq(from_sq)),
                                theta
                        )
                )
        );
}

}       // namespace ulo
}       // namespace grid
}       // namespace dctl
