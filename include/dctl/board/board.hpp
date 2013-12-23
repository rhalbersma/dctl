#pragma once
#include <array>                                // array
#include <cstddef>                              // size_t
#include <cstdint>                              // uint64_t
#include <limits>                               // digits
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/range/counting_range.hpp>       // counting_range
#include <dctl/angle.hpp>                       // Angle, inverse
#include <dctl/bit/bit_set.hpp>                 // bit_set
#include <dctl/grid/coordinates.hpp>            // Square, coord_from_sq, sq_from_coord, rotate
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/grid/shift_size.hpp>             // shift_size
#include <dctl/grid/orientation.hpp>            // SizeMinimizingOrientation, Make
#include <dctl/node/side.hpp>                   // black, white
#include <dctl/utility/make_array.hpp>          // make_array

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
        using set_type = bit::bit_set<int, Block, Nb>;

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
