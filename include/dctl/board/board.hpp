#pragma once
#include <array>                        // array
#include <cstddef>                      // size_t
#include <cstdint>                      // uint64_t
#include <limits>                       // digits
#include <dctl/angle.hpp>               // Angle, inverse
#include <dctl/bit/bit_set.hpp>         // bit_set
#include <dctl/grid/coordinates.hpp>    // Square, coord_from_sq, sq_from_coord, rotate
#include <dctl/grid/grid.hpp>           // Grid
#include <dctl/grid/shift_size.hpp>     // shift_size
#include <dctl/grid/orientation.hpp>    // SizeMinimizingOrientation, Make
#include <dctl/node/side.hpp>           // black, white
#include <dctl/utility/make_array.hpp>  // make_array

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

        using InternalGrid = grid::Make<Dimensions, edge_columns, orientation>;
        using ExternalGrid = grid::Grid<Dimensions, 0>;

        using Block = uint64_t;
        static constexpr auto needed = InternalGrid::size / std::numeric_limits<Block>::digits + 1;
        static constexpr auto Nb = needed;
        static constexpr auto N = Nb * std::numeric_limits<Block>::digits;
        using set_type = bit::bit_set<int, Block, Nb>;

        static constexpr auto shift_size(Angle const& direction)
        {
                return grid::shift_size<InternalGrid>(direction);
        }

        static constexpr auto begin() noexcept
        {
                return 0;
        }

        static constexpr auto end() noexcept
        {
                return ExternalGrid::size;
        }

        static constexpr auto is_valid(int square) noexcept
        {
                return begin() <= square && square < end();
        }

private:
        template<class DestGrid, class FromSquare>
        static constexpr auto transform(FromSquare const& from_sq, Angle const& theta)
        {
                return grid::ulo::sq_from_coord(
                        grid::ulo_from_sco<DestGrid>(
                                rotate(
                                        sco_from_ulo(coord_from_sq(from_sq)),
                                        theta
                                )
                        )
                );
        }

        static constexpr int init_bit_from_square(int n) noexcept
        {
                return transform<InternalGrid>(grid::ulo::Square<ExternalGrid>{n}, orientation).value();
        }

        static constexpr int init_square_from_bit(int n) noexcept
        {
                return transform<ExternalGrid>(grid::ulo::Square<InternalGrid>{n}, inverse(orientation)).value();
        }

        static constexpr std::array<int, N> table_bit_from_square = make_array<N>(init_bit_from_square);
        static constexpr std::array<int, N> table_square_from_bit = make_array<N>(init_square_from_bit);

public:
        static constexpr auto bit_from_square(int n) noexcept
        {
                return table_bit_from_square[static_cast<std::size_t>(n)];
        }

        static constexpr auto square_from_bit(int n) noexcept
        {
                return table_square_from_bit[static_cast<std::size_t>(n)];
        }

        /* NOTE: for C++11/14, constexpr predicate != lambda expression */
        template<class Predicate>
        static constexpr auto copy_if(Predicate pred) noexcept
        {
                set_type result{};
                for (auto sq = 0; sq < ExternalGrid::size; ++sq)
                        if (pred(grid::ulo::Square<ExternalGrid>{sq}))
                                result.set(bit_from_square(sq));
                return result;
        }
};

template<class D, bool O>
constexpr bool Board<D, O>::is_orthogonal_captures;

template<class D, bool O>
constexpr int Board<D, O>::edge_columns;

template<class D, bool O>
constexpr Angle Board<D, O>::orientation;

template<class D, bool O>
constexpr std::array<int, Board<D, O>::N> Board<D, O>::table_bit_from_square;

template<class D, bool O>
constexpr std::array<int, Board<D, O>::N> Board<D, O>::table_square_from_bit;

}       // namespace board
}       // namespace dctl
