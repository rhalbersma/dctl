#pragma once
#include <boost/mpl/bool.hpp>                   // bool_
#include <boost/mpl/int.hpp>                    // int_
#include <boost/preprocessor/repetition.hpp>    // BOOST_PP_ENUM
#include <dctl/angle/degrees.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/grid/dimensions/transform.hpp>
#include <dctl/grid/edge.hpp>
#include <dctl/grid/grid.hpp>
#include <dctl/grid/predicates.hpp>
#include <dctl/grid/shift_size.hpp>
#include <dctl/board/mask/init.hpp>
#include <dctl/node/side.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace board {

template<class Dimensions, class Edge = grid::DoubleColumnEdge>
struct Board
:
        public Dimensions, public Edge
{
        using ReOrientedDimensions = typename mpl::lazy::rotate<
                Dimensions, angle::Degrees<Edge::orientation>
        >::type;
public:
        // internal and external grids
        using InternalGrid = grid::Grid<ReOrientedDimensions, Edge>;
        using ExternalGrid = grid::Grid<Dimensions, grid::ColumnLessEdge>;

        using bit_type = BitBoard;

        static constexpr auto shift_size(int direction) noexcept
        {
                return grid::shift_size<InternalGrid>(direction);
        }

        template<int Direction>
        using jump_start = mask::init<
                grid::is_jump_start, Board, angle::Degrees< angle::rotate(Direction, Edge::orientation) >
        >;

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

        static constexpr auto square2bit(int number) noexcept
        {
                return SQUARE2BIT[number];
        }

        static constexpr auto bit2square(int number) noexcept
        {
                return BIT2SQUARE[number];
        }

        // essential bitboard mask

        static constexpr BitBoard squares = mask::init< grid::is_square, Board >::value;

#define DCTL_PP_INITIAL_MASK(z, i, data)      \
        mask::init< grid::is_initial, Board, boost::mpl::bool_<data>, boost::mpl::int_<i> >::value

        static constexpr BitBoard initial_mask[][5] =
        {
                { BOOST_PP_ENUM(5, DCTL_PP_INITIAL_MASK, Side::black) },
                { BOOST_PP_ENUM(5, DCTL_PP_INITIAL_MASK, Side::white) }
        };

#undef DCTL_PP_INITIAL_MASK

#define DCTL_PP_ROW_MASK(z, i, data)      \
        mask::init< grid::is_row, Board, boost::mpl::bool_<data>, boost::mpl::int_<i> >::value

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

#define DCTL_PP_COL_MASK(z, i, data)      \
        mask::init< grid::is_col, Board, boost::mpl::bool_<data>, boost::mpl::int_<i> >::value

        static constexpr BitBoard col_mask[][12] =
        {
                { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::black) },
                { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::white) }
        };

#undef DCTL_PP_COL_MASK

        // auxiliary bitboard mask
        static BitBoard const DOUBLE_NEAREST_NEIGHBOR_MAGIC[];  // shifting bits in 2 directions
        static BitBoard const QUAD_NEAREST_NEIGHBOR_MAGIC;      // shifting bits in 4 directions

private:

#define DCTL_PP_SQUARE2BIT(z, i, data) \
        board::square_to_bit< Board >(i).value()

        static constexpr int SQUARE2BIT[] = {
                BOOST_PP_ENUM(64, DCTL_PP_SQUARE2BIT, ~)
        };

#undef DCTL_PP_SQUARE2BIT

#define DCTL_PP_BIT2SQUARE(z, i, data) \
        board::bit_to_square< Board >(i).value()

        static constexpr int BIT2SQUARE[] = {
                BOOST_PP_ENUM(64, DCTL_PP_BIT2SQUARE, ~)
        };

#undef DCTL_PP_BIT2SQUARE

};

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::squares;

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::initial_mask[][5];

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::promotion_mask[][2];

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::row_mask[][12];

template<class Dimensions, class Edge>
constexpr BitBoard Board<Dimensions, Edge>::col_mask[][12];

template<class Dimensions, class Edge>
constexpr int Board<Dimensions, Edge>::SQUARE2BIT[];

template<class Dimensions, class Edge>
constexpr int Board<Dimensions, Edge>::BIT2SQUARE[];

template<class Dimensions, class Edge>
BitBoard const Board<Dimensions, Edge>::DOUBLE_NEAREST_NEIGHBOR_MAGIC[] = {
        (bit::singlet<BitBoard>(1)) ^ (bit::singlet<BitBoard>(1 + (InternalGrid::left_down::value  << 1))),
        (bit::singlet<BitBoard>(0)) ^ (bit::singlet<BitBoard>(0 + (InternalGrid::right_down::value << 1)))
};

template<class Dimensions, class Edge>
BitBoard const Board<Dimensions, Edge>::QUAD_NEAREST_NEIGHBOR_MAGIC =
        DOUBLE_NEAREST_NEIGHBOR_MAGIC[0] ^ DOUBLE_NEAREST_NEIGHBOR_MAGIC[1]
;

}       // namespace board
}       // namespace dctl
