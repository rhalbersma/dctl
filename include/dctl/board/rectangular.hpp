#pragma once
#include <dctl/board/angle.hpp>                         // Angle, inverse
#include <dctl/board/detail/coordinates.hpp>            // to_llo, transform
#include <dctl/board/detail/dimensions.hpp>             // dimensions
#include <dctl/board/detail/grid.hpp>                   // InnerGrid, OuterGrid
#include <dctl/board/detail/optimal_orientation.hpp>    // size_Minimizing_orientation
#include <dctl/color_piece.hpp>                               // black, white
#include <dctl/utility/fill_array.hpp>                  // fill_array
#include <xstd/bitset.hpp>                              // bitset
#include <xstd/cstddef.hpp>                             // _zu
#include <xstd/cstdint.hpp>                             // uint_fast
#include <xstd/limits.hpp>                              // align_on
#include <boost/range/irange.hpp>                       // irange
#include <array>                                        // array
#include <cstddef>                                      // size_t
#include <iomanip>                                      // setfill
#include <limits>                                       // digits
#include <sstream>                                      // stringstream

namespace dctl {
namespace board {
namespace block_adl {

template
<
        std::size_t Width,
        std::size_t Height,
        bool IsInverted = false,
        bool IsOrthogonalJump = false
>
class Rectangular
{
public:
        using type = Rectangular;
        static constexpr auto width              = Width;
        static constexpr auto height             = Height;
        static constexpr auto is_inverted        = IsInverted;
        static constexpr auto is_orthogonal_jump = IsOrthogonalJump;

        static constexpr auto edge = is_orthogonal_jump ? 2 : 1;
        static constexpr auto inner_grid = detail::InnerGrid{detail::dimensions{width, height, is_inverted}};
        static constexpr auto orientation = detail::optimal_orientation(detail::OuterGrid{inner_grid, edge});
        static constexpr auto outer_grid = detail::OuterGrid{rotate(inner_grid, orientation), edge};

private:
        static constexpr auto NumBits = outer_grid.size();
        static constexpr auto NumSquares = inner_grid.size();

public:
        static constexpr auto lower_left_is_square() noexcept { return inner_grid.lower_left_is_square(); }
        static constexpr auto upper_left_is_square() noexcept { return inner_grid.upper_left_is_square(); }

        static constexpr auto edge_le() noexcept { return inner_grid.edge_le(); }
        static constexpr auto edge_lo() noexcept { return inner_grid.edge_lo(); }

        static constexpr auto size() noexcept
        {
                return NumSquares;
        }

        static constexpr auto bits() noexcept
        {
                return NumBits;
        }

        using    set_type = xstd::bitset<xstd::align_on(NumBits, 64)>;
        using square_type = xstd::uint_fast_t<set_type::size()>;

        static auto squares() noexcept
        {
                using namespace xstd::support_literals;
                return boost::irange(0_zu, size());
        }

        static auto bitnrs() noexcept
        {
                using namespace xstd::support_literals;
                return boost::irange(0_zu, bits());
        }

        static auto numeric_from_bit(std::size_t const n)
        {
                assert(n < NumBits);
                std::stringstream sstr;
                sstr << std::setfill('0') << std::setw(2) << square_from_bit(n) + 1;
                return sstr.str();
        }

        static auto algebraic_from_bit(std::size_t const n)
        {
                assert(n < NumBits);
                std::stringstream sstr;
                auto coord = detail::to_llo(square_from_bit(n), inner_grid);
                sstr << column_label(coord.x) << row_label(coord.y);
                return sstr.str();
        }
private:
        static constexpr auto init_bit_from_square(std::size_t const sq) noexcept
        {
                assert(sq < NumSquares);
                return detail::transform(sq, inner_grid, outer_grid, inverse(orientation));
        }

        static constexpr auto init_square_from_bit(std::size_t const n) noexcept
        {
                assert(n < NumBits);
                return detail::transform(n, outer_grid, inner_grid, orientation);
        }

        static constexpr auto column_label(std::size_t const n) noexcept
        {
                assert(n < width);
                return static_cast<char>('a' + n);
        }

        static constexpr auto row_label(std::size_t const n) noexcept
        {
                assert(n < height);
                return 1 + n;
        }

        static constexpr std::array<std::size_t, NumSquares>
        table_bit_from_square = fill_array<NumSquares>(init_bit_from_square);

        static constexpr std::array<std::size_t, NumBits>
        table_square_from_bit = fill_array<NumBits>(init_square_from_bit);
public:
        static constexpr auto bit_from_square(std::size_t const sq)
        {
                assert(sq < NumSquares);
                return table_bit_from_square[sq];
        }

        static constexpr auto square_from_bit(std::size_t const n)
        {
                assert(n < NumBits);
                return table_square_from_bit[n];
        }

        static constexpr auto is_square(detail::coordinates<detail::upper_left> const& coord) noexcept
        {
                return ((coord.x % 2) ^ (coord.y % 2)) != upper_left_is_square();
        }

        static constexpr auto to_square(detail::coordinates<detail::upper_left> const& coord) noexcept
        {
                return detail::to_square(coord, inner_grid);
        }
};

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr Angle
Rectangular<Width, Height, Inverted, OrthogonalCaptures>::orientation;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr detail::InnerGrid
Rectangular<Width, Height, Inverted, OrthogonalCaptures>::inner_grid;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr detail::OuterGrid
Rectangular<Width, Height, Inverted, OrthogonalCaptures>::outer_grid;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr std::array<std::size_t, Rectangular<Width, Height, Inverted, OrthogonalCaptures>::NumSquares>
Rectangular<Width, Height, Inverted, OrthogonalCaptures>::table_bit_from_square;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr std::array<std::size_t, Rectangular<Width, Height, Inverted, OrthogonalCaptures>::NumBits>
Rectangular<Width, Height, Inverted, OrthogonalCaptures>::table_square_from_bit;

}       // namespace block_adl

using block_adl::Rectangular;

}       // namespace board
}       // namespace dctl
