#pragma once
#include <dctl/board/angle.hpp>               // Angle
#include <dctl/board/origin.hpp>        // ScreenCentered, UpperLeft, LowerLeft
#include <cassert>                      // assert
#include <stdexcept>                    // invalid_argument
#include <tuple>                        // forward_as_tuple

namespace dctl {
namespace board {

template<class Origin>
class Coordinates
{
        int x_, y_;
public:
        using origin_type = Origin;

        constexpr Coordinates(int a, int b) noexcept : x_{a}, y_{b} {}

        constexpr auto x() const noexcept { return x_; }
        constexpr auto y() const noexcept { return y_; }
};

template<class Origin>
constexpr auto operator==(Coordinates<Origin> const& lhs, Coordinates<Origin> const& rhs) noexcept
{
        return
                std::forward_as_tuple(lhs.x(), lhs.y()) ==
                std::forward_as_tuple(rhs.x(), rhs.y())
        ;
}

template<class Origin>
constexpr auto operator!=(Coordinates<Origin> const& lhs, Coordinates<Origin> const& rhs) noexcept
{
        return !(lhs == rhs);
}

constexpr auto rotate(Coordinates<origin::ScreenCentered> const& coord, Angle const& a)
{
        switch (a) {
        case   0 : return coord;
        case  90 : return Coordinates<origin::ScreenCentered>{ -coord.y(),  coord.x() };
        case 180 : return Coordinates<origin::ScreenCentered>{ -coord.x(), -coord.y() };
        case 270 : return Coordinates<origin::ScreenCentered>{  coord.y(), -coord.x() };
        default  : return throw std::invalid_argument("Rotations of Coordinates objects shall be in multiples of 90 degrees."), coord;
        }
}

constexpr auto swap_llo_ulo(int value, int bound) noexcept
{
        assert(value < bound);
        return (bound - 1) - value;
}

constexpr auto sco_from_ulo(int value, int bound) noexcept
{
        return 2 * value - (bound - 1);
}

constexpr auto ulo_from_sco(int value, int bound) noexcept
{
        return (value + (bound - 1)) / 2;
}

template<class Grid>
constexpr auto to_ulo(Coordinates<origin::ScreenCentered> const& coord, Grid const& grid) noexcept
{
        return Coordinates<origin::UpperLeft>
        {
                ulo_from_sco(coord.x(), grid.width() ),
                ulo_from_sco(coord.y(), grid.height())
        };
}

template<class Grid>
constexpr auto to_sco(Coordinates<origin::UpperLeft> const& coord, Grid const& grid) noexcept
{
        return Coordinates<origin::ScreenCentered>
        {
                sco_from_ulo(coord.x(), grid.width() ),
                sco_from_ulo(coord.y(), grid.height())
        };
}

template<class Grid>
constexpr auto to_llo(Coordinates<origin::UpperLeft> const& coord, Grid const& grid) noexcept
{
        return Coordinates<origin::LowerLeft>
        {
                coord.x(),
                swap_llo_ulo(coord.y(), grid.height())
        };
}

template<class Grid>
constexpr auto to_ulo(Coordinates<origin::LowerLeft> const& coord, Grid const& grid) noexcept
{
        return Coordinates<origin::UpperLeft>
        {
                coord.x(),
                swap_llo_ulo(coord.y(), grid.height())
        };
}

template<class Grid>
constexpr auto to_llo(Coordinates<origin::ScreenCentered> const& coord, Grid const& grid) noexcept
{
        return to_llo(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto to_sco(Coordinates<origin::LowerLeft> const& coord, Grid const& grid) noexcept
{
        return to_sco(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto to_square(Coordinates<origin::UpperLeft> const& coord, Grid const& grid)
{
        auto const col_mod = coord.x() % 2;
        auto const row_mod = coord.y() % 2;
        assert(row_mod ^ col_mod == !grid.ul_parity());

        auto const col_div = coord.x() / 2;
        assert(2 * col_div + col_mod == coord.x());

        auto const row_div = coord.y() / 2;
        assert(2 * row_div + row_mod == coord.y());

        auto const sq_base = row_mod ? grid.edge_lo() : grid.edge_le();
        auto const sq_offset = sq_base + col_div;

        return row_div * grid.modulo() + sq_offset;
}

template<class Grid>
constexpr auto to_square(Coordinates<origin::LowerLeft> const& coord, Grid const& grid)
{
        return to_square(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto to_square(Coordinates<origin::ScreenCentered> const& coord, Grid const& grid)
{
        return to_square(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto to_ulo(std::size_t sq, Grid const& grid)
{
        auto const row_div   = sq / grid.modulo();
        auto const sq_offset = sq % grid.modulo();
        assert(row_div * grid.modulo() + sq_offset == sq);

        auto const row_mod = sq_offset >= grid.edge_lo();
        auto const col_mod = row_mod ^ !grid.ul_parity();
        assert(row_mod ^ col_mod == !grid.ul_parity());

        auto const sq_base = row_mod ? grid.edge_lo() : grid.edge_le();
        auto const col_div = sq_offset - sq_base;

        return Coordinates<origin::UpperLeft>
        {
                static_cast<int>(2 * col_div + col_mod),
                static_cast<int>(2 * row_div + row_mod)
        };
}

template<class Grid>
constexpr auto to_llo(std::size_t sq, Grid const& grid)
{
        return to_llo(to_ulo(sq, grid), grid);
}

template<class Grid>
constexpr auto to_sco(std::size_t sq, Grid const& grid)
{
        return to_sco(to_ulo(sq, grid), grid);
}

template<class FromGrid, class DestGrid>
constexpr auto transform(std::size_t sq, FromGrid const& from, DestGrid const& dest, Angle a)
{
        // sq | to_sco(from) | rotate(a) | to_square(dest)
        return to_square(rotate(to_sco(sq, from), a), dest);
}

}       // namespace board
}       // namespace dctl
