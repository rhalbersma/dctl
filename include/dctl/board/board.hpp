#pragma once
#include <array>                                // array
#include <cstddef>                              // size_t
#include <cstdint>                              // uint64_t
#include <iomanip>
#include <limits>                               // digits
#include <sstream>
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/range/counting_range.hpp>       // counting_range
#include <dctl/angle.hpp>                       // Angle, inverse
#include <dctl/bit.hpp>                         // Set
#include <dctl/grid/coordinates.hpp>            // Square, coord_from_sq, sq_from_coord, rotate
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/grid/shift_size.hpp>             // shift_size
#include <dctl/grid/orientation.hpp>            // SizeMinimizingOrientation, Make
#include <dctl/position/side.hpp>               // black, white
#include <dctl/utility/make_array.hpp>          // make_array
#include <dctl/board/algebraic.hpp>

namespace dctl {
namespace board {

template<class Dimensions, bool IsOrthogonalCaptures = true>
struct Board
:
        public Dimensions
{
public:
        static constexpr auto is_orthogonal_captures = IsOrthogonalCaptures;
        static constexpr auto edge_columns = IsOrthogonalCaptures ? 2 : 1;
        static constexpr auto orientation = Angle{grid::SizeMinimizingOrientation<Dimensions, edge_columns>::value};

        using internal_grid = grid::Make<Dimensions, edge_columns, orientation>;
        using external_grid = grid::Grid<Dimensions, 0>;

private:
        using Block = uint64_t;
        static constexpr auto Nb = internal_grid::size / std::numeric_limits<Block>::digits + 1;
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
                return boost::counting_range(*begin(), *end());
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
                auto from_coord = llo_from_ulo(coord_from_sq(from_sq));
                sstr << board::Labels<Board>::col[from_coord.col()] << board::Labels<Board>::row[from_coord.row()];
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

template<class Dim, bool Orth>
constexpr bool Board<Dim, Orth>::is_orthogonal_captures;

template<class Dim, bool Orth>
constexpr int Board<Dim, Orth>::edge_columns;

template<class Dim, bool Orth>
constexpr Angle Board<Dim, Orth>::orientation;

template<class Dim, bool Orth>
constexpr std::array<int, Board<Dim, Orth>::N>
Board<Dim, Orth>::table_bit_from_square;

template<class Dim, bool Orth>
constexpr std::array<int, Board<Dim, Orth>::N>
Board<Dim, Orth>::table_square_from_bit;

}       // namespace board
}       // namespace dctl
