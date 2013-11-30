#pragma once
#include <dctl/angle.hpp>               // Angle, _deg, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/grid/coordinates.hpp>
#include <dctl/grid/dimensions.hpp>
#include <dctl/grid/grid.hpp>

namespace dctl {
namespace grid {

struct is_square
{
        template<class Square>
        constexpr auto operator()(Square const& sq) const noexcept
        {
                using Grid = typename Square::grid_type;

                return 0 <= sq.value() && sq.value() < Grid::size;
        }
};

struct is_initial
{
        template<class Square>
        constexpr auto operator()(bool color, int separation, Square const& sq) const noexcept
        {
                using Grid = typename Square::grid_type;

                auto const row = grid::detail::decentralize(grid::sqtocoord(sq).row(), Grid::height);
                auto const min_row = color ? Grid::height - (Grid::height - separation) / 2 : 0;
                auto const max_row = color ? Grid::height : (Grid::height - separation) / 2;
                return min_row <= row && row < max_row;
        }
};

struct is_row
{
        template<class Square>
        constexpr auto operator()(bool color, int row, Square const& sq) const noexcept
        {
                using Grid = typename Square::grid_type;

                return grid::detail::decentralize(grid::sqtocoord(sq).row(), Grid::height) == (color ? Grid::height - 1 - row : row);
        }
};

struct is_column
{
        template<class Square>
        constexpr auto operator()(bool color, int column, Square const& sq) const noexcept
        {
                using Grid = typename Square::grid_type;

                return grid::detail::decentralize(grid::sqtocoord(sq).col(), Grid::width) == (color ? Grid::width - 1 - column : column);
        }
};

struct is_jump_start
{
        template<class Square>
        constexpr auto operator()(Angle const& theta, Square const& sq) noexcept
        {
                using Grid = typename Square::grid_type;

                auto const offset = is_diagonal(theta) ? 2 : 4;

                auto const row = grid::detail::decentralize(grid::sqtocoord(sq).row(), Grid::height);
                auto const min_row = is_up(theta) ? offset : 0;
                auto const max_row = Grid::height - (is_down(theta)? offset : 0);

                auto const col = grid::detail::decentralize(grid::sqtocoord(sq).col(), Grid::width);
                auto const min_col = is_left(theta) ? offset : 0;
                auto const max_col = Grid::width - (is_right(theta) ? offset : 0);

                return (min_row <= row && row < max_row) && (min_col <= col && col < max_col);
        }
};

namespace detail {

constexpr auto is_even_or_odd_jump_difference(int R0, int C0, int R1, int C1) noexcept
{
        // a diagonal or orthogonal man jump between square <FROM> and square <DEST> is possible if
        // either both row and column numbers difference == 0 mod 4 (even number of jumps)
        // or both row and column numbers difference == 2 mod 4 (odd number of jumps)
        return (!R0 && !C0) || (!R1 && !C1);
}

constexpr auto is_jump_difference(int delta_row, int delta_col) noexcept
{
        return is_even_or_odd_jump_difference(
                delta_row % 4,
                delta_col % 4,
                (delta_row + 2) % 4,
                (delta_col + 2) % 4
        );
}

template<class Coordinates>
constexpr auto is_jump_connected(Coordinates const& from_coord, Coordinates const& dest_coord) noexcept
{
        return is_jump_difference(
                from_coord.row() - dest_coord.row(),
                from_coord.col() - dest_coord.col()
        );
}

template<class Square>
constexpr auto is_jump_group(Square const& from_sq, Square const& dest_sq) noexcept
{
        return is_jump_connected(
                sqtocoord( Square{from_sq.value()} ),
                sqtocoord( Square{dest_sq.value()} )
        );
}

}       // namespace detail

template<class Square>
constexpr auto is_jump_group(int group, int square) noexcept
{
        return detail::is_jump_group( Square{group}, Square{square} );
}

}       // namespace grid
}       // namespace dctl
