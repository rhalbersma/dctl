#pragma once
#include <dctl/board/angle.hpp> // angle
#include <xstd/type_traits.hpp> // value_t
#include <cassert>              // assert
#include <cstddef>              // size_t
#include <stdexcept>            // invalid_argument
#include <tuple>                // tie

namespace dctl {
namespace board {
namespace detail {

struct upper_left      { using value_type = std::size_t; };
struct lower_left      { using value_type = std::size_t; };
struct screen_centered { using value_type = int;         };

template<class Origin>
struct coordinates
{
        using value_type = xstd::value_t<Origin>;
        value_type const x;
        value_type const y;
};

template<class Origin>
constexpr auto tied(coordinates<Origin> const coord) noexcept
{
        return std::tie(coord.x, coord.y);
}

template<class Origin>
constexpr auto operator==(coordinates<Origin> const lhs, coordinates<Origin> const rhs) noexcept
{
        return tied(lhs) == tied(rhs);
}

template<class Origin>
constexpr auto operator!=(coordinates<Origin> const lhs, coordinates<Origin> const rhs) noexcept
{
        return !(lhs == rhs);
}

constexpr auto rotate(coordinates<screen_centered> const coord, angle const a)
        -> coordinates<screen_centered>
{
        switch (a.degrees()) {
        case   0 : return coord;
        case  90 : return { -coord.y,  coord.x };
        case 180 : return { -coord.x, -coord.y };
        case 270 : return {  coord.y, -coord.x };
        default  : return static_cast<void>(throw std::invalid_argument("Not a multiple of 90 degrees.")), coord;
        }
}

constexpr auto swap_llo_ulo(std::size_t const value, std::size_t const bound) noexcept
{
        assert(value < bound);
        return (bound - 1) - value;
}

constexpr auto sco_from_ulo(std::size_t const value, std::size_t const bound) noexcept
{
        return static_cast<int>(2 * value - (bound - 1));
}

constexpr auto ulo_from_sco(int const value, std::size_t const bound) noexcept
{
        return (value + (bound - 1)) / 2;
}

template<class Grid>
constexpr auto to_ulo(coordinates<screen_centered> const coord, Grid const grid) noexcept
        -> coordinates<upper_left>
{
        return { ulo_from_sco(coord.x, grid.width ()), ulo_from_sco(coord.y, grid.height()) };
}

template<class Grid>
constexpr auto to_sco(coordinates<upper_left> const coord, Grid const grid) noexcept
        -> coordinates<screen_centered>
{
        return { sco_from_ulo(coord.x, grid.width ()), sco_from_ulo(coord.y, grid.height()) };
}

template<class Grid>
constexpr auto to_llo(coordinates<upper_left> const coord, Grid const grid) noexcept
        -> coordinates<lower_left>
{
        return { coord.x, swap_llo_ulo(coord.y, grid.height()) };
}

template<class Grid>
constexpr auto to_ulo(coordinates<lower_left> const coord, Grid const grid) noexcept
        -> coordinates<upper_left>
{
        return { coord.x, swap_llo_ulo(coord.y, grid.height()) };
}

template<class Grid>
constexpr auto to_llo(coordinates<screen_centered> const coord, Grid const grid) noexcept
{
        return to_llo(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto to_sco(coordinates<lower_left> const coord, Grid const grid) noexcept
{
        return to_sco(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto to_square(coordinates<upper_left> const coord, Grid const grid)
{
        //auto const col_mod = coord.x() % 2;
        auto const row_mod = coord.y % 2;
        //assert(row_mod ^ col_mod == !grid.upper_left_is_square());

        auto const col_div = coord.x / 2;
        //assert(2 * col_div + col_mod == coord.x());

        auto const row_div = coord.y / 2;
        //assert(2 * row_div + row_mod == coord.y());

        auto const sq_base = row_mod ? grid.edge_lo() : grid.edge_le();
        auto const sq_offset = sq_base + col_div;

        return row_div * grid.modulo() + sq_offset;
}

template<class Grid>
constexpr auto to_square(coordinates<lower_left> const coord, Grid const grid)
{
        return to_square(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto to_square(coordinates<screen_centered> const coord, Grid const grid)
{
        return to_square(to_ulo(coord, grid), grid);
}

template<class Grid>
constexpr auto to_ulo(std::size_t const sq, Grid const grid)
        -> coordinates<upper_left>
{
        auto const row_div   = sq / grid.modulo();
        auto const sq_offset = sq % grid.modulo();
        assert(row_div * grid.modulo() + sq_offset == sq);

        auto const row_mod = sq_offset >= grid.edge_lo();
        auto const col_mod = row_mod ^ !grid.upper_left_is_square();
        assert((row_mod ^ col_mod) == !grid.upper_left_is_square());

        auto const sq_base = row_mod ? grid.edge_lo() : grid.edge_le();
        auto const col_div = sq_offset - sq_base;

        return { 2 * col_div + col_mod, 2 * row_div + row_mod };
}

template<class Grid>
constexpr auto to_llo(std::size_t const sq, Grid const grid)
{
        return to_llo(to_ulo(sq, grid), grid);
}

template<class Grid>
constexpr auto to_sco(std::size_t const sq, Grid const grid)
{
        return to_sco(to_ulo(sq, grid), grid);
}

template<class FromGrid, class DestGrid>
constexpr auto transform(std::size_t const sq, FromGrid const from, DestGrid const dest, angle const a)
{
        // sq.to_sco(from).rotate(a).to_square(dest)
        return to_square(rotate(to_sco(sq, from), a), dest);
}

}       // namespace detail
}       // namespace board
}       // namespace dctl
