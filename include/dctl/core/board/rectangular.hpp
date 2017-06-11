#pragma once
#include <dctl/core/board/angle.hpp>                    // angle, inverse
#include <dctl/core/board/coordinates.hpp>              // to_llo, transform
#include <dctl/core/board/detail/bit_layout.hpp>        // dimensions, InnerGrid, bit_layout
#include <dctl/core/state/color_piece.hpp>              // black, white
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
                [g = detail::bit_layout{inner_grid, edge}]
                (auto const lhs, auto const rhs) {
                        return rotate(g, lhs).size() < rotate(g, rhs).size();
                }
        );
        constexpr static auto outer_grid = detail::bit_layout{rotate(inner_grid, orientation), edge};

private:
        constexpr static auto NumBits = outer_grid.size();
        constexpr static auto NumSquares = inner_grid.size();

        constexpr static auto lower_left_is_square() noexcept { return inner_grid.lower_left_is_square(); }
        constexpr static auto upper_left_is_square() noexcept { return inner_grid.upper_left_is_square(); }

public:
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
                assert(static_cast<std::size_t>(n) < NumBits);
                constexpr auto file_label = [](auto const f) { return static_cast<char>('a' + f); };
                constexpr auto rank_label = [](auto const r) { return 1 + r;                      };
                std::stringstream sstr;
                auto const coord = to_llo(square_from_bit(n), inner_grid);
                sstr << file_label(coord.x) << rank_label(coord.y);
                return sstr.str();
        }
private:
        constexpr static auto table_bit_from_square = []() {
                auto table = std::array<int, NumSquares>{};
                for (auto sq = 0; sq < NumSquares; ++sq) {
                        table[static_cast<std::size_t>(sq)] =
                                transform(sq, inner_grid, outer_grid, inverse(orientation))
                        ;
                }
                return table;
        }();

        constexpr static auto table_square_from_bit = []() {
                auto table = std::array<int, NumBits>{};
                for (auto n = 0; n < NumBits; ++n) {
                        table[static_cast<std::size_t>(n)] =
                                transform(n, outer_grid, inner_grid, orientation)
                        ;
                }
                return table;
        }();
public:
        constexpr static auto bit_from_square(int const sq)
        {
                assert(static_cast<std::size_t>(sq) < NumSquares);
                return table_bit_from_square[static_cast<std::size_t>(sq)];
        }

        constexpr static auto square_from_bit(int const n)
        {
                assert(static_cast<std::size_t>(n) < NumBits);
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
