#pragma once
#include <dctl/grid/ulo.hpp>

namespace dctl {
namespace grid {

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
                coord_from_sq( Square{from_sq.value()} ),
                coord_from_sq( Square{dest_sq.value()} )
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
