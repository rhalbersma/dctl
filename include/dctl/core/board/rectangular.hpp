#pragma once
#include <dctl/core/board/angle.hpp>                    // angle, inverse
#include <dctl/core/board/coordinates.hpp>       // to_llo, transform
#include <dctl/core/board/detail/dimensions.hpp>        // dimensions
#include <dctl/core/board/detail/grid.hpp>              // InnerGrid, OuterGrid
#include <dctl/core/state/color_piece.hpp>                    // black, white
#include <dctl/util/fill_array.hpp>                     // fill_array
#include <xstd/cstdint.hpp>                             // uint_fast
#include <xstd/cstdlib.hpp>                             // align_on
#include <xstd/int_set.hpp>                             // int_set
#include <algorithm>                                    // min
#include <array>                                        // array
#include <cstddef>                                      // size_t
#include <iomanip>                                      // setfill
#include <limits>                                       // digits
#include <sstream>                                      // stringstream

namespace dctl::core {
namespace board {
namespace block_adl {

template
<
        int Width,
        int Height,
        bool IsInverted = false,
        bool IsOrthogonalJump = false
>
class rectangular
{
public:
        using type = rectangular;
        constexpr static auto width              = Width;
        constexpr static auto height             = Height;
        constexpr static auto is_inverted        = IsInverted;
        constexpr static auto is_orthogonal_jump = IsOrthogonalJump;

        constexpr static auto edge = is_orthogonal_jump ? 2 : 1;
        constexpr static auto inner_grid = detail::InnerGrid{detail::dimensions{width, height, is_inverted}};
        constexpr static angle orientation = std::min(
                { 0_deg, 90_deg, 180_deg, 270_deg },
                [g = detail::OuterGrid{inner_grid, edge}]
                (auto const lhs, auto const rhs) {
                        return rotate(g, lhs).size() < rotate(g, rhs).size();
                }
        );
        constexpr static auto outer_grid = detail::OuterGrid{rotate(inner_grid, orientation), edge};

private:
        constexpr static auto NumBits = outer_grid.size();
        constexpr static auto NumSquares = inner_grid.size();

public:
        constexpr static auto lower_left_is_square() noexcept { return inner_grid.lower_left_is_square(); }
        constexpr static auto upper_left_is_square() noexcept { return inner_grid.upper_left_is_square(); }

        constexpr static auto edge_le() noexcept { return inner_grid.edge_le(); }
        constexpr static auto edge_lo() noexcept { return inner_grid.edge_lo(); }

        constexpr static auto size() noexcept
        {
                return NumSquares;
        }

        constexpr static auto bits() noexcept
        {
                return NumBits;
        }

        using    set_type = xstd::int_set<xstd::align_on(NumBits, xstd::int_set<NumBits>::capacity())>;
        using square_type = xstd::uint_fast_t<set_type::max_size()>;

        static auto numeric_from_bit(int const n)
        {
                assert(n < NumBits);
                std::stringstream sstr;
                sstr << std::setfill('0') << std::setw(2) << square_from_bit(n) + 1;
                return sstr.str();
        }

        static auto algebraic_from_bit(int const n)
        {
                assert(n < NumBits);
                std::stringstream sstr;
                auto coord = to_llo(square_from_bit(n), inner_grid);
                sstr << column_label(coord.x) << row_label(coord.y);
                return sstr.str();
        }
private:
        constexpr static auto init_bit_from_square(int const sq) noexcept
        {
                assert(sq < NumSquares);
                return transform(sq, inner_grid, outer_grid, inverse(orientation));
        }

        constexpr static auto init_square_from_bit(int const n) noexcept
        {
                assert(n < NumBits);
                return transform(n, outer_grid, inner_grid, orientation);
        }

        constexpr static auto column_label(int const n) noexcept
        {
                assert(n < width);
                return static_cast<char>('a' + n);
        }

        constexpr static auto row_label(int const n) noexcept
        {
                assert(n < height);
                return 1 + n;
        }

        constexpr static std::array<int, NumSquares>
        table_bit_from_square = fill_array<NumSquares>(init_bit_from_square);

        constexpr static std::array<int, NumBits>
        table_square_from_bit = fill_array<NumBits>(init_square_from_bit);
public:
        constexpr static auto bit_from_square(int const sq)
        {
                assert(sq < NumSquares);
                return table_bit_from_square[static_cast<std::size_t>(sq)];
        }

        constexpr static auto square_from_bit(int const n)
        {
                assert(n < NumBits);
                return table_square_from_bit[static_cast<std::size_t>(n)];
        }

        constexpr static auto is_square(coordinates<upper_left> const& coord) noexcept
        {
                return ((coord.x % 2) ^ (coord.y % 2)) != upper_left_is_square();
        }

        constexpr static auto to_square(coordinates<upper_left> const& coord) noexcept
        {
                return core::to_square(coord, inner_grid);
        }
};

}       // namespace block_adl

using block_adl::rectangular;

}       // namespace board
}       // namespace dctl::core
