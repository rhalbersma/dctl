#pragma once
#include <dctl/angle.hpp>                       // Angle, _deg
#include <dctl/grid/coordinates.hpp>
#include <dctl/grid/dimensions.hpp>             // Rotate_t
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/grid/shift_size.hpp>             // shift_size
#include <dctl/grid/predicates.hpp>
#include <dctl/node/side.hpp>
#include <dctl/bit/bit_set.hpp>
#include <dctl/utility/make_array.hpp>

namespace dctl {
namespace board {

template<class Dimensions, int EdgeColumns = 2, int Orientation = 0_deg>
struct Board
:
        public Dimensions
{
public:
        static constexpr auto edge_columns = EdgeColumns;
        static constexpr auto orientation = Angle{Orientation};

        // internal and external grids
        using InternalGrid = grid::Grid<grid::Rotate<Dimensions, orientation>, edge_columns>;
        using ExternalGrid = grid::Grid<Dimensions, 0>;

        using bit_type = BitSet;

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
                return grid::sq_from_coord<DestGrid>(rotate(grid::coord_from_sq(from_sq), theta));
        }

        static constexpr int init_bit_from_square(int n) noexcept
        {
                return transform<InternalGrid>(grid::Square<ExternalGrid>{n}, orientation).value();
        }

        static constexpr int init_square_from_bit(int n) noexcept
        {
                return transform<ExternalGrid>(grid::Square<InternalGrid>{n}, inverse(orientation)).value();
        }

        static constexpr auto table_bit_from_square = make_array<128>(init_bit_from_square);
        static constexpr auto table_square_from_bit = make_array<128>(init_square_from_bit);

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
                BitSet result;
                for (auto sq = 0; sq < ExternalGrid::size; ++sq)
                        if (pred(grid::Square<ExternalGrid>{sq}))
                                result.set(bit_from_square(sq));
                return result;
        }
};

template<class D, int E, int O>
constexpr std::array<int, 128> Board<D, E, O>::table_bit_from_square;

template<class D, int E, int O>
constexpr std::array<int, 128> Board<D, E, O>::table_square_from_bit;

}       // namespace board
}       // namespace dctl
