#pragma once
#include <dctl/angle.hpp>                               // Angle
#include <dctl/grid/coordinates/llo.hpp>                // Coordinates
#include <dctl/grid/coordinates/sco.hpp>                // Square, Coordinates
#include <dctl/grid/coordinates/ulo.hpp>                // Coordinates
#include <dctl/grid/coordinates/detail/transform.hpp>   // llo_from_sco, sco_from_ulo, ulo_from_sco

namespace dctl {
namespace grid {
namespace ulo {

// conversions between Squares and Coordinates with Upper Left Origin

template<class Grid>
constexpr Coordinates<Grid> coord_from_sq(Square<Grid> const& square) noexcept
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

        // 2x the row pairs + the row parity
        auto const ROW = 2 * Q + P;

        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        auto const COL = 2 * R + (P ^ !Grid::parity);

        return { ROW, COL };
}

template<class Grid>
constexpr auto sq_from_coord(Coordinates<Grid> const& coord) noexcept
{
        // row parity
        auto const P = coord.row() % 2;

        // number of row pairs
        auto const Q = coord.row() / 2;

        // the left edge
        auto const L = P ? Grid::edge_lo : Grid::edge_le;

        // number of column pairs
        auto const S = coord.col() / 2;

        // squares from the left edge
        auto const R = (L + S) % Grid::modulo;

        auto const NUM = Grid::modulo * Q + R;

        return Square<Grid>{ NUM };
}

}       // namespace ulo

// conversions between Coordinates with Upper Left Origin and Screen Centered Origin

template<class Grid>
constexpr llo::Coordinates<Grid> llo_from_ulo(ulo::Coordinates<Grid> const& coord)
{
        return {
                detail::llo_from_ulo(coord.row(), Grid::height),
                                     coord.col()
        };
}

template<class Grid>
constexpr sco::Coordinates sco_from_ulo(ulo::Coordinates<Grid> const& coord)
{
        return {
                detail::sco_from_ulo(coord.row(), Grid::height),
                detail::sco_from_ulo(coord.col(), Grid::width )
        };
}

template<class Grid>
constexpr ulo::Coordinates<Grid> ulo_from_sco(sco::Coordinates const& coord)
{
        return {
                detail::ulo_from_sco(coord.row(), Grid::height),
                detail::ulo_from_sco(coord.col(), Grid::width )
        };
}

namespace sco {

// rotations among Coordinates with Screen Centered Origin

inline
constexpr auto rotate(Coordinates const& coord, Angle const& theta)
{
        switch (theta) {
        case   0: return coord;
        case  90: return Coordinates{  coord.col(), -coord.row() };
        case 180: return Coordinates{ -coord.row(), -coord.col() };
        case 270: return Coordinates{ -coord.col(),  coord.row() };
        default: return throw std::invalid_argument("Coordinates rotation angles shall be a multiple of 90 degrees."), coord;
        }
}

}       // namespace sco
}       // namespace grid
}       // namespace dctl
