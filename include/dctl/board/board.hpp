#pragma once
#include <boost/preprocessor/repetition.hpp>    // BOOST_PP_ENUM
#include <dctl/angle/degrees.hpp>
#include <dctl/grid/coordinates.hpp>
#include <dctl/grid/dimensions.hpp>             // Rotate_t
#include <dctl/grid/grid.hpp>                   // Grid
#include <dctl/grid/shift_size.hpp>             // shift_size
#include <dctl/grid/predicates.hpp>
#include <dctl/node/side.hpp>
#include <dctl/utility/range.hpp>               // is_element
#include <dctl/bit/bitboard.hpp>                // BitBoard

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

        using bit_type = BitBoard;

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
                return util::is_element(square, {begin(), end()});
        }

private:
        template<class DestGrid, class FromSquare>
        static constexpr auto transform(FromSquare const& from_sq, Angle const& theta)
        {
                return grid::coordtosq<DestGrid>(rotate(grid::sqtocoord(from_sq), theta));
        }

        static constexpr auto init_square2bit(int n) noexcept
        {
                return transform<InternalGrid>(grid::Square<ExternalGrid>{n}, orientation).value();
        }

        static constexpr auto init_bit2square(int n) noexcept
        {
                return transform<ExternalGrid>(grid::Square<InternalGrid>{n}, inverse(orientation)).value();
        }

#define DCTL_PP_SQUARE2BIT(z, i, data) init_square2bit(i)

        static constexpr int table_square2bit[] =
        {
                BOOST_PP_ENUM(64, DCTL_PP_SQUARE2BIT, ~)
        };

#undef DCTL_PP_SQUARE2BIT

#define DCTL_PP_BIT2SQUARE(z, i, data) init_bit2square(i)

        static constexpr int table_bit2square[] =
        {
                BOOST_PP_ENUM(64, DCTL_PP_BIT2SQUARE, ~)
        };

#undef DCTL_PP_BIT2SQUARE

public:
        static constexpr auto square2bit(int number) noexcept
        {
                return table_square2bit[number];
        }

        static constexpr auto bit2square(int number) noexcept
        {
                return table_bit2square[number];
        }

private:
        /* NOTE: for C++11/14, constexpr predicate != lambda expression */
        template<class Predicate>
        static constexpr auto copy_if(Predicate pred) noexcept
        {
                bit::bit_set<int, uint64_t, 1> result;
                for (auto sq = 0; sq < ExternalGrid::size; ++sq)
                        if (pred(grid::Square<ExternalGrid>{sq}))
                                result.set(square2bit(sq));
                return result.data();
        }

public:
        static constexpr BitBoard squares = copy_if(grid::is_square{});

#define DCTL_PP_INITIAL_MASK(z, i, data) copy_if(grid::is_initial<i, data>{})

        static constexpr BitBoard initial_mask[][5] =
        {
                { BOOST_PP_ENUM(5, DCTL_PP_INITIAL_MASK, Side::black) },
                { BOOST_PP_ENUM(5, DCTL_PP_INITIAL_MASK, Side::white) }
        };

#undef DCTL_PP_INITIAL_MASK

#define DCTL_PP_ROW_MASK(z, i, data) copy_if(grid::is_row<i, data>{})

        static constexpr BitBoard promotion_mask[][2] =
        {
                { BOOST_PP_ENUM(2, DCTL_PP_ROW_MASK, Side::white) },
                { BOOST_PP_ENUM(2, DCTL_PP_ROW_MASK, Side::black) }
        };

        static constexpr BitBoard row_mask[][12] =
        {
                { BOOST_PP_ENUM(12, DCTL_PP_ROW_MASK, Side::black) },
                { BOOST_PP_ENUM(12, DCTL_PP_ROW_MASK, Side::white) }
        };

#undef DCTL_PP_ROW_MASK

#define DCTL_PP_COL_MASK(z, i, data) copy_if(grid::is_col<i, data>{})

        static constexpr BitBoard col_mask[][12] =
        {
                { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::black) },
                { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::white) }
        };

#undef DCTL_PP_COL_MASK

private:

#define DCTL_PP_JUMP_START(z, i, data) copy_if(grid::is_jump_start<rotate(Angle{i * 45_deg}, orientation)>{})

        static constexpr BitBoard table_jump_start[] =
        {
                BOOST_PP_ENUM(8, DCTL_PP_JUMP_START, ~)
        };

#undef DCTL_PP_JUMP_START

public:
        static constexpr auto jump_start(Angle const& direction) noexcept
        {
                return table_jump_start[direction / 45_deg];
        }
};

template<class D, int E, int O> constexpr int Board<D, E, O>::table_square2bit[];
template<class D, int E, int O> constexpr int Board<D, E, O>::table_bit2square[];

template<class D, int E, int O> constexpr BitBoard Board<D, E, O>::squares;
template<class D, int E, int O> constexpr BitBoard Board<D, E, O>::initial_mask[][5];
template<class D, int E, int O> constexpr BitBoard Board<D, E, O>::promotion_mask[][2];
template<class D, int E, int O> constexpr BitBoard Board<D, E, O>::row_mask[][12];
template<class D, int E, int O> constexpr BitBoard Board<D, E, O>::col_mask[][12];
template<class D, int E, int O> constexpr BitBoard Board<D, E, O>::table_jump_start[];

}       // namespace board
}       // namespace dctl
