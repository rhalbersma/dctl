#pragma once
#include <dctl/board/angle.hpp>                 // angle, inverse
#include <dctl/board/algebraic.hpp>             // Labels
#include <dctl/board/dimensions.hpp>            // Dimensions
#include <dctl/board/coordinates.hpp>           // Square, ulo_from_sq, sq_from_ulo, rotate
#include <dctl/board/grid.hpp>                  // Grid
#include <dctl/board/detail/orientation.hpp>    // size_minimizing_orientation
#include <dctl/board/shift.hpp>                 // Shift
#include <dctl/color.hpp>                       // black, white
#include <dctl/utility/make_array.hpp>          // make_array
#include <xstd/bitset.hpp>                      // bitset
#include <xstd/cstddef.hpp>                     // _z
#include <xstd/limits.hpp>                      // align_on
#include <range/v3/all.hpp>                     // view::iota
#include <array>                                // array
#include <cstddef>                              // size_t
#include <iomanip>                              // setfill
#include <limits>                               // digits
#include <sstream>                              // stringstream

namespace dctl {
namespace board {

template
<
        int Width,
        int Height,
        bool Inverted = false,
        bool OrthogonalCaptures = true
>
class rectangular
{
        static_assert(0 <= Width);
        static_assert(0 <= Height);
public:
        static constexpr auto is_orthogonal_captures = OrthogonalCaptures;
        static constexpr auto edge = OrthogonalCaptures ? 2 : 1;
        static constexpr auto inner_grid = InnerGrid{Dimensions{Width, Height, Inverted}};
        static constexpr auto orientation = size_minimizing_orientation(OuterGrid{inner_grid, edge});

private:
        static constexpr auto outer_grid = OuterGrid{rotate(inner_grid, orientation), edge};
        static constexpr auto NumBits = outer_grid.size();
        static constexpr auto NumSquares = inner_grid.size();

public:
        static constexpr auto width()     noexcept { return inner_grid.width();     }
        static constexpr auto height()    noexcept { return inner_grid.height();    }
        static constexpr auto inverted()  noexcept { return inner_grid.inverted();  }
        static constexpr auto ll_parity() noexcept { return inner_grid.ll_parity(); }
        static constexpr auto ul_parity() noexcept { return inner_grid.ul_parity(); }

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

        using set_type = xstd::bitset<xstd::align_on(NumBits, 64)>;

        static constexpr auto shift_size(angle const direction)
        {
                return Shift{outer_grid}(direction);
        }

        static auto squares() noexcept
        {
                return ranges::view::iota(0, size());
        }

        static auto bitnrs() noexcept
        {
                return ranges::view::iota(0, bits());
        }

        static auto numeric_from_bit(std::size_t n)
        {
                std::stringstream sstr;
                sstr << std::setfill('0') << std::setw(2) << square_from_bit(n) + 1;
                return sstr.str();
        }

        static auto algebraic_from_bit(std::size_t n)
        {
                std::stringstream sstr;
                auto coord = to_llo(square_from_bit(n), inner_grid);
                sstr << Labels<rectangular>::col[coord.x()] << Labels<rectangular>::row[coord.y()];
                return sstr.str();
        }
private:
        static constexpr auto init_bit_from_square(int sq) noexcept
        {
                return static_cast<std::size_t>(transform(sq, inner_grid, outer_grid, orientation));
        }

        static constexpr auto init_square_from_bit(std::size_t n) noexcept
        {
                return transform(static_cast<int>(n), outer_grid, inner_grid, inverse(orientation));
        }

        static constexpr std::array<std::size_t, NumSquares>
        table_bit_from_square = make_array<NumSquares>(init_bit_from_square);

        static constexpr std::array<int, NumBits>
        table_square_from_bit = make_array<NumBits>(init_square_from_bit);

public:
        static constexpr auto bit_from_square(int sq)
        {
                return table_bit_from_square[static_cast<std::size_t>(sq)];
        }

        static constexpr auto square_from_bit(std::size_t n)
        {
                return table_square_from_bit[n];
        }

        static constexpr auto is_square(Coordinates<origin::upper_left> const& coord) noexcept
        {
                return ((coord.x() % 2) ^ (coord.y() % 2)) != ul_parity();
        }

        static constexpr auto to_square(Coordinates<origin::upper_left> const& coord) noexcept
        {
                return board::to_square(coord, inner_grid);
        }
};

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr angle
rectangular<Width, Height, Inverted, OrthogonalCaptures>::orientation;

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr InnerGrid
rectangular<Width, Height, Inverted, OrthogonalCaptures>::inner_grid;

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr OuterGrid
rectangular<Width, Height, Inverted, OrthogonalCaptures>::outer_grid;

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr std::array<std::size_t, rectangular<Width, Height, Inverted, OrthogonalCaptures>::NumSquares>
rectangular<Width, Height, Inverted, OrthogonalCaptures>::table_bit_from_square;

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr std::array<int, rectangular<Width, Height, Inverted, OrthogonalCaptures>::NumBits>
rectangular<Width, Height, Inverted, OrthogonalCaptures>::table_square_from_bit;

}       // namespace board
}       // namespace dctl
