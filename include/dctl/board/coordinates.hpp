#pragma once
#include <dctl/angle.hpp>               // Angle
#include <dctl/board/origin.hpp>        // ScreenCentered, UpperLeft, LowerLeft
#include <cassert>                      // assert
#include <stdexcept>                    // invalid_argument
#include <tuple>                        // forward_as_tuple

namespace dctl {
namespace board {

template<class T>
struct Coordinates
{
        int x, y;
};

template<class Origin>
constexpr auto
operator==(Coordinates<Origin> const& lhs, Coordinates<Origin> const& rhs) noexcept
{
        return
                std::forward_as_tuple(lhs.x, lhs.y) ==
                std::forward_as_tuple(rhs.x, rhs.y)
        ;
}

template<class Origin>
constexpr auto
operator!=(Coordinates<Origin> const& lhs, Coordinates<Origin> const& rhs) noexcept
{
        return !(lhs == rhs);
}

inline
constexpr auto
rotate(Coordinates<origin::ScreenCentered> const& coord, Angle const& theta)
{
        switch (theta) {
        case   0: return coord;
        case  90: return Coordinates<origin::ScreenCentered>{ -coord.y,  coord.x };
        case 180: return Coordinates<origin::ScreenCentered>{ -coord.x, -coord.y };
        case 270: return Coordinates<origin::ScreenCentered>{  coord.y, -coord.x };
        default: return throw std::invalid_argument("Rotation angles shall be a multiple of 90 degrees."), coord;
        }
}

template<class T>
constexpr auto swap_llo_ulo(T const& value, T const& bound) noexcept
{
        assert(value < bound);
        return (bound - 1) - value;
}

template<class T>
constexpr auto sco_from_ulo(T const& value, T const& bound) noexcept
{
        return 2 * value - (bound - 1);
}

template<class T>
constexpr auto ulo_from_sco(T const& value, T const& bound) noexcept
{
        return (value + (bound - 1)) / 2;
}

template<class Grid>
constexpr auto
to_ulo(Coordinates<origin::ScreenCentered> const& coord, Grid const& grid)
{
        return Coordinates<origin::UpperLeft>{
                ulo_from_sco(coord.x, grid.width ),
                ulo_from_sco(coord.y, grid.height)
        };
}

template<class Grid>
constexpr auto
to_sco(Coordinates<origin::UpperLeft> const& coord, Grid const& grid)
{
        return Coordinates<origin::ScreenCentered>{
                sco_from_ulo(coord.x, grid.width ),
                sco_from_ulo(coord.y, grid.height)
        };
}

template<class Grid>
constexpr auto
to_llo(Coordinates<origin::UpperLeft> const& coord, Grid const& grid)
{
        return Coordinates<origin::LowerLeft>{
                coord.x,
                swap_llo_ulo(coord.y, grid.height)
        };
}

template<class Grid>
constexpr auto
to_ulo(Coordinates<origin::LowerLeft> const& coord, Grid const& grid)
{
        return Coordinates<origin::UpperLeft>{
                coord.x,
                swap_llo_ulo(coord.y, grid.height)
        };
}

template<class Grid>
constexpr auto
to_llo(Coordinates<origin::ScreenCentered> const& coord, Grid const& grid)
{
        return to_llo(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto
to_sco(Coordinates<origin::LowerLeft> const& coord, Grid const& grid)
{
        return to_sco(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto
to_square(Coordinates<origin::UpperLeft> const& coord, Grid const& grid) noexcept
{
        auto const row_parity = coord.y % 2;
        auto const row_pairs = coord.y / 2;
        auto const left_edge = row_parity ? grid.edge_lo() : grid.edge_le();
        auto const column_pairs = coord.x / 2;
        auto const squares = (left_edge + column_pairs) % grid.modulo();
        auto const NUM = grid.modulo() * row_pairs + squares;

        return NUM;
}

template<class Grid>
constexpr auto
to_square(Coordinates<origin::LowerLeft> const& coord, Grid const& grid) noexcept
{
        return to_square(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto
to_square(Coordinates<origin::ScreenCentered> const& coord, Grid const& grid) noexcept
{
        return to_square(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto
to_ulo(int square, Grid const& grid) noexcept
{
        auto const row_pairs = square / grid.modulo();
        auto const R0 = square % grid.modulo();
        auto const R1 = R0 - grid.edge_lo();
        auto const row_parity = R1 >= 0;
        auto const R = row_parity ? R1 : R0;
        auto const X = 2 * R + (row_parity ^ !ul_parity(grid));
        auto const Y = 2 * row_pairs + row_parity;

        return Coordinates<origin::UpperLeft>{ X, Y };
}

template<class Grid>
constexpr auto
to_llo(int square, Grid const& grid) noexcept
{
        return to_llo(to_ulo(square, grid), grid);
}

template<class Grid>
constexpr auto
to_sco(int square, Grid const& grid) noexcept
{
        return to_sco(to_ulo(square, grid), grid);
}

template<class InGrid, class OutGrid>
constexpr auto
transform(int sq, InGrid const& in_grid, OutGrid out_grid, Angle const& theta)
{
        return to_square(rotate(to_sco(sq, in_grid), theta), out_grid);
}

}       // namespace board
}       // namespace dctl
