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
                return grid::coordtosq<DestGrid>(rotate(grid::sqtocoord(from_sq), theta));
        }

        static constexpr int init_square2bit(int n) noexcept
        {
                return transform<InternalGrid>(grid::Square<ExternalGrid>{n}, orientation).value();
        }

        static constexpr int init_bit2square(int n) noexcept
        {
                return transform<ExternalGrid>(grid::Square<InternalGrid>{n}, inverse(orientation)).value();
        }

        static constexpr auto table_square2bit = make_array<128>(init_square2bit);
        static constexpr auto table_bit2square = make_array<128>(init_bit2square);

public:
        static constexpr auto square2bit(int n) noexcept
        {
                return table_square2bit[static_cast<std::size_t>(n)];
        }

        static constexpr auto bit2square(int n) noexcept
        {
                return table_bit2square[static_cast<std::size_t>(n)];
        }

        /* NOTE: for C++11/14, constexpr predicate != lambda expression */
        template<class Predicate>
        static constexpr auto copy_if(Predicate pred) noexcept
        {
                BitSet result;
                for (auto sq = 0; sq < ExternalGrid::size; ++sq)
                        if (pred(grid::Square<ExternalGrid>{sq}))
                                result.set(square2bit(sq));
                return result;
        }
};

template<class D, int E, int O> constexpr std::array<int, 128> Board<D, E, O>::table_square2bit;
template<class D, int E, int O> constexpr std::array<int, 128> Board<D, E, O>::table_bit2square;

}       // namespace board
}       // namespace dctl
