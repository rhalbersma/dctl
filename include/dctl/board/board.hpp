#pragma once
#include <dctl/angle.hpp>                       // Angle, inverse
#include <dctl/bit.hpp>                         // Set
#include <dctl/board/algebraic.hpp>
#include <dctl/grid/coordinates.hpp>            // Square, ulo_from_sq, sq_from_ulo, rotate
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/grid/shift_size.hpp>             // shift_size
#include <dctl/grid/orientation.hpp>            // SizeMinimizingOrientation, Make
#include <dctl/position/color.hpp>               // black, white
#include <dctl/utility/make_array.hpp>          // make_array
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/range/irange.hpp>               // irange
#include <array>                                // array
#include <cstddef>                              // size_t
#include <cstdint>                              // uint64_t
#include <iomanip>
#include <limits>                               // digits
#include <sstream>

namespace dctl {
namespace board {

template
<
        int Width,
        int Height,
        bool Inverted = false,
        bool OrthogonalCaptures = true
>
class Board
:
        public grid::Dimensions<Width, Height, Inverted>
{
private:
        using Dimensions = grid::Dimensions<Width, Height, Inverted>;

public:
        static constexpr auto is_orthogonal_captures = OrthogonalCaptures;
        static constexpr auto edge_columns = OrthogonalCaptures ? 2 : 1;
        static constexpr auto orientation = Angle{grid::SizeMinimizingOrientation<Dimensions, edge_columns>::value};

        using internal_grid = grid::Make<Dimensions, edge_columns, orientation>;
        using external_grid = grid::Grid<Dimensions, 0>;

private:
        using Block = uint64_t;
        static constexpr auto Nb = (internal_grid::size - 1) / std::numeric_limits<Block>::digits + 1;
        static constexpr auto N = Nb * std::numeric_limits<Block>::digits;

public:
        using set_type = bit::Set<int, Block, Nb>;

        static constexpr auto size() noexcept
        {
                return external_grid::size;
        }

        static constexpr auto shift_size(Angle const& direction)
        {
                return grid::shift_size<internal_grid>(direction);
        }

        static auto begin() noexcept
        {
                return boost::counting_iterator<int>{0};
        }

        static auto end() noexcept
        {
                return boost::counting_iterator<int>{size()};
        }

        static auto squares() noexcept
        {
                return boost::irange(*begin(), *end());
        }

        static auto numeric_from_bit(int n)
        {
                std::stringstream sstr;
                sstr << std::setfill('0') << std::setw(2) << square_from_bit(n) + 1;
                return sstr.str();
        }

        static auto algebraic_from_bit(int n)
        {
                std::stringstream sstr;
                auto from_sq = grid::ulo::Square<external_grid>{square_from_bit(n)};
                auto from_coord = llo_from_ulo(ulo_from_sq(from_sq));
                sstr << board::Labels<Board>::col[get_x(from_coord)] << board::Labels<Board>::row[get_y(from_coord)];
                return sstr.str();
        }
private:
        static constexpr int
        init_bit_from_square(int n) noexcept
        {
                return grid::ulo::square_from_square<internal_grid>(
                        grid::ulo::Square<external_grid>{n}, orientation
                ).value();
        }

        static constexpr int
        init_square_from_bit(int n) noexcept
        {
                return grid::ulo::square_from_square<external_grid>(
                        grid::ulo::Square<internal_grid>{n}, inverse(orientation)
                ).value();
        }

        static constexpr std::array<int, N>
        table_bit_from_square = make_array<N>(init_bit_from_square);

        static constexpr std::array<int, N>
        table_square_from_bit = make_array<N>(init_square_from_bit);

public:
        static constexpr auto bit_from_square(int n) noexcept
        {
                return table_bit_from_square[static_cast<std::size_t>(n)];
        }

        static constexpr auto square_from_bit(int n) noexcept
        {
                return table_square_from_bit[static_cast<std::size_t>(n)];
        }
};

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr bool
Board<Width, Height, Inverted, OrthogonalCaptures>::is_orthogonal_captures;

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr int
Board<Width, Height, Inverted, OrthogonalCaptures>::edge_columns;

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr Angle
Board<Width, Height, Inverted, OrthogonalCaptures>::orientation;

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr std::array<int, Board<Width, Height, Inverted, OrthogonalCaptures>::N>
Board<Width, Height, Inverted, OrthogonalCaptures>::table_bit_from_square;

template<int Width, int Height, bool Inverted, bool OrthogonalCaptures>
constexpr std::array<int, Board<Width, Height, Inverted, OrthogonalCaptures>::N>
Board<Width, Height, Inverted, OrthogonalCaptures>::table_square_from_bit;

}       // namespace board
}       // namespace dctl
