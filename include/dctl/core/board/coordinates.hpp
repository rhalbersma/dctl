#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>    // angle
#include <dctl/util/type_traits.hpp>    // value_t
#include <cassert>                      // assert
#include <tuple>                        // tie

namespace dctl::core {

struct upper_left      {};
struct lower_left      {};
struct screen_centered {};

template<class Origin>
struct coordinates
{
        int const x;
        int const y;

        bool operator==(coordinates const&) const = default;
};

constexpr auto rotate(coordinates<screen_centered> const coord, angle const a) noexcept
        -> coordinates<screen_centered>
{
        switch (a.value()) {
        case   0: return coord;
        case  90: return { -coord.y,  coord.x };
        case 180: return { -coord.x, -coord.y };
        case 270: return {  coord.y, -coord.x };
        default: assert(false); return coord;
        }
}

constexpr auto swap_llo_ulo(int const value, int const bound) noexcept
{
        // TODO: assert(value < bound);
        return (bound - 1) - value;
}

constexpr auto sco_from_ulo(int const value, int const bound) noexcept
{
        return 2 * value - (bound - 1);
}

constexpr auto ulo_from_sco(int const value, int const bound) noexcept
{
        return (value + (bound - 1)) / 2;
}

template<class Grid>
constexpr auto to_ulo(coordinates<screen_centered> const coord, Grid const grid) noexcept
        -> coordinates<upper_left>
{
        return { ulo_from_sco(coord.x, grid.width()), ulo_from_sco(coord.y, grid.height()) };
}

template<class Grid>
constexpr auto to_sco(coordinates<upper_left> const coord, Grid const grid) noexcept
        -> coordinates<screen_centered>
{
        return { sco_from_ulo(coord.x, grid.width()), sco_from_ulo(coord.y, grid.height()) };
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
        auto const col_mod [[maybe_unused]] = coord.x % 2;
        auto const row_mod = coord.y % 2;
        assert((row_mod ^ col_mod) == !grid.upper_left_is_square());

        auto const col_div = coord.x / 2;
        assert(2 * col_div + col_mod == coord.x);

        auto const row_div = coord.y / 2;
        assert(2 * row_div + row_mod == coord.y);

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
constexpr auto to_ulo(int const sq, Grid const grid)
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
constexpr auto to_llo(int const sq, Grid const grid)
{
        return to_llo(to_ulo(sq, grid), grid);
}

template<class Grid>
constexpr auto to_sco(int const sq, Grid const grid)
{
        return to_sco(to_ulo(sq, grid), grid);
}

template<class FromGrid, class DestGrid>
constexpr auto transform(int const sq, FromGrid const from, DestGrid const dest, angle const a)
{
        return to_square(rotate(to_sco(sq, from), a), dest);
}

}       // namespace dctl::core
