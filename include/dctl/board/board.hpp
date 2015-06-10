#pragma once
#include <dctl/angle.hpp>                       // Angle, inverse
#include <dctl/board/algebraic.hpp>             // Labels
#include <dctl/board/dimensions.hpp>            // Dimensions
#include <dctl/board/coordinates.hpp>           // Square, ulo_from_sq, sq_from_ulo, rotate
#include <dctl/board/grid.hpp>                  // Grid
#include <dctl/board/detail/orientation.hpp>    // size_minimizing_orientation
#include <dctl/board/shift.hpp>                 // Shift
#include <dctl/color.hpp>                       // black, white
#include <dctl/utility/make_array.hpp>          // make_array
#include <xstd/cstddef.hpp>                     // _z
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
        std::size_t Width,
        std::size_t Height,
        bool Inverted = false,
        bool OrthogonalCaptures = true
>
class Board
{
private:
        static constexpr auto dimensions = Dimensions{Width, Height, Inverted};

public:
        static constexpr auto is_orthogonal_captures = OrthogonalCaptures;
        static constexpr std::size_t edge_columns = OrthogonalCaptures ? 2 : 1;
        static constexpr auto inner_grid = InnerGrid{dimensions};
        static constexpr Angle orientation = size_minimizing_orientation(OuterGrid{inner_grid, edge_columns});

private:
        static constexpr auto outer_grid = OuterGrid{rotate(inner_grid, orientation), edge_columns};
        static constexpr auto NumBits = outer_grid.size();
        static constexpr auto NumSquares = inner_grid.size();

public:
        static constexpr auto width()     noexcept { return inner_grid.width();     }
        static constexpr auto height()    noexcept { return inner_grid.height();    }
        static constexpr auto inverted()  noexcept { return inner_grid.inverted();  }
        static constexpr auto ll_parity() noexcept { return inner_grid.ll_parity(); }
        static constexpr auto ul_parity() noexcept { return inner_grid.ul_parity(); }

        static constexpr auto modulo()  noexcept { return inner_grid.modulo();  }
        static constexpr auto edge_re() noexcept { return inner_grid.edge_re(); }
        static constexpr auto edge_ro() noexcept { return inner_grid.edge_ro(); }
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

        static constexpr auto shift_size(Angle direction)
        {
                return Shift{outer_grid}(direction);
        }

        static auto squares() noexcept
        {
                using namespace xstd::support_literals;
                return ranges::view::iota(0_z, size());
        }

        static auto bitnrs() noexcept
        {
                using namespace xstd::support_literals;
                return ranges::view::iota(0_z, bits());
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
                auto coord = to_llo(square_from_bit(n), outer_grid);
                sstr << Labels<Board>::col[coord.x] << Labels<Board>::row[coord.y];
                return sstr.str();
        }
private:
        static constexpr auto init_bit_from_square(std::size_t n) noexcept
        {
                return transform(n, inner_grid, outer_grid, orientation);
        }

        static constexpr auto init_square_from_bit(std::size_t n) noexcept
        {
                return transform(n, outer_grid, inner_grid, inverse(orientation));
        }

        static constexpr std::array<std::size_t, NumSquares>
        table_bit_from_square = make_array<NumSquares>(init_bit_from_square);

        static constexpr std::array<std::size_t, NumBits>
        table_square_from_bit = make_array<NumBits>(init_square_from_bit);

public:
        static constexpr auto bit_from_square(std::size_t n) noexcept
        {
                return table_bit_from_square[n];
        }

        static constexpr auto square_from_bit(std::size_t n)
        {
                return table_square_from_bit[n];
        }
};

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr bool
Board<Width, Height, Inverted, OrthogonalCaptures>::is_orthogonal_captures;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr std::size_t
Board<Width, Height, Inverted, OrthogonalCaptures>::edge_columns;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr Angle
Board<Width, Height, Inverted, OrthogonalCaptures>::orientation;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr InnerGrid
Board<Width, Height, Inverted, OrthogonalCaptures>::inner_grid;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr OuterGrid
Board<Width, Height, Inverted, OrthogonalCaptures>::outer_grid;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr std::array<std::size_t, Board<Width, Height, Inverted, OrthogonalCaptures>::NumSquares>
Board<Width, Height, Inverted, OrthogonalCaptures>::table_bit_from_square;

template<std::size_t Width, std::size_t Height, bool Inverted, bool OrthogonalCaptures>
constexpr std::array<std::size_t, Board<Width, Height, Inverted, OrthogonalCaptures>::NumBits>
Board<Width, Height, Inverted, OrthogonalCaptures>::table_square_from_bit;

}       // namespace board
}       // namespace dctl
