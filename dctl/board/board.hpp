#pragma once
#include <boost/mpl/arithmetic.hpp>             // plus
#include <boost/mpl/bool.hpp>                   // bool_
#include <boost/mpl/int.hpp>                    // int_
#include <boost/mpl/placeholders.hpp>
#include <boost/preprocessor/repetition.hpp>    // BOOST_PP_ENUM
#include <dctl/angle/degrees.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/dimensions/transform.hpp>
#include <dctl/board/ghosts.hpp>
#include <dctl/board/grid.hpp>
#include <dctl/board/mask/init.hpp>
#include <dctl/board/mask/predicates.hpp>
#include <dctl/board/shift_size.hpp>
#include <dctl/node/side.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace board {

template
<
        typename Dimensions,            // height, width and parity
        typename Layout = Ghosts<>      // columns and internal rotation
>
struct Board
:
        public Dimensions,
        public Layout
{
public:
        // external and internal grids
        typedef Grid<Dimensions, no_ghosts> ExternalGrid;
        typedef Grid<typename
                mpl::lazy::rotate<
                        Dimensions, typename
                        Layout::full_angle
                >::type, typename
                Layout::columns
        > InternalGrid;

        typedef BitBoard bit_type;

        template<typename Direction>
        struct shift_size
        :
                ShiftSize<InternalGrid, Direction>
        {};

        template<typename Direction>
        struct jump_start
        :
                mask::init<
                        mask::is_jump_start<
                                Board, boost::mpl::_1,
                                mpl::lazy::rotate< Direction, typename Layout::full_angle >
                        >
                >
        {};

        static int begin()
        {
                return 0;
        }

        static int end()
        {
                return ExternalGrid::size::value;
        }

        static bool is_valid(int square)
        {
                return begin() <= square && square < end();
        }

       static int square2bit(int number)
        {
                return SQUARE2BIT[number];
        }

        static int bit2square(int number)
        {
                return BIT2SQUARE[number];
        }

        // essential bitboard mask
        static BitBoard const squares;                          // bit mask of legal squares, excluding borders
        static BitBoard const initial_mask[][5];                // initial position
        static BitBoard const promotion_mask[][2];              // promotion zones
        static BitBoard const row_mask[][12];                   // bit mask for the rows
        static BitBoard const col_mask[][12];                   // bit mask for the columns

        // auxiliary bitboard mask
        static BitBoard const QUAD_NEAREST_NEIGHBOR_MAGIC;      // shifting bits in 4 directions
        static BitBoard const DOUBLE_NEAREST_NEIGHBOR_MAGIC[];  // shifting bits in 2 directions

private:
        // square to bit and bit to square conversion tables
        static int const SQUARE2BIT[];                          // convert a square to a bit
        static int const BIT2SQUARE[];                          // convert a bit to a square
};

template<typename Dimensions, typename Layout>
BitBoard const Board<Dimensions, Layout>::squares = mask::init< mask::is_square<Board, boost::mpl::_1> >::value;

#define DCTL_PP_INITIAL_MASK(z, i, data)      \
        mask::init< mask::is_initial< Board, boost::mpl::_1, boost::mpl::bool_<data>, boost::mpl::int_<i> > >::value

template<typename Dimensions, typename Layout>
BitBoard const Board<Dimensions, Layout>::initial_mask[][5] = {
        { BOOST_PP_ENUM(5, DCTL_PP_INITIAL_MASK, Side::black) },
        { BOOST_PP_ENUM(5, DCTL_PP_INITIAL_MASK, Side::white) }
};

#undef DCTL_PP_INITIAL_MASK

#define DCTL_PP_ROW_MASK(z, i, data)      \
        mask::init< mask::is_row< Board, boost::mpl::_1, boost::mpl::bool_<data>, boost::mpl::int_<i> > >::value

template<typename Dimensions, typename Layout>
BitBoard const Board<Dimensions, Layout>::promotion_mask[][2] = {
        { BOOST_PP_ENUM(2, DCTL_PP_ROW_MASK, Side::white) },
        { BOOST_PP_ENUM(2, DCTL_PP_ROW_MASK, Side::black) }
};

template<typename Dimensions, typename Layout>
BitBoard const Board<Dimensions, Layout>::row_mask[][12] = {
        { BOOST_PP_ENUM(12, DCTL_PP_ROW_MASK, Side::black) },
        { BOOST_PP_ENUM(12, DCTL_PP_ROW_MASK, Side::white) }
};

#undef DCTL_PP_ROW_MASK

#define DCTL_PP_COL_MASK(z, i, data)      \
        mask::init< mask::is_col< Board, boost::mpl::_1, boost::mpl::bool_<data>, boost::mpl::int_<i> > >::value

template<typename Dimensions, typename Layout>
BitBoard const Board<Dimensions, Layout>::col_mask[][12] = {
        { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::black) },
        { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::white) }
};

#undef DCTL_PP_COL_MASK

#define DCTL_PP_SQUARE2BIT(z, i, data) \
        square_to_bit< Board, boost::mpl::int_<i> >::type::number::value

template<typename Dimensions, typename Layout>
int const Board<Dimensions, Layout>::SQUARE2BIT[] = {
        BOOST_PP_ENUM(64, DCTL_PP_SQUARE2BIT, ~)
};

#undef DCTL_PP_SQUARE2BIT

#define DCTL_PP_BIT2SQUARE(z, i, data) \
        bit_to_square< Board, boost::mpl::int_<i> >::type::number::value

template<typename Dimensions, typename Layout>
int const Board<Dimensions, Layout>::BIT2SQUARE[] = {
        BOOST_PP_ENUM(64, DCTL_PP_BIT2SQUARE, ~)
};

#undef DCTL_PP_BIT2SQUARE

template<typename Dimensions, typename Layout>
BitBoard const Board<Dimensions, Layout>::DOUBLE_NEAREST_NEIGHBOR_MAGIC[] = {
        (bit::singlet<BitBoard>(1)) ^ (bit::singlet<BitBoard>(1 + (InternalGrid::left_down::value  << 1))),
        (bit::singlet<BitBoard>(0)) ^ (bit::singlet<BitBoard>(0 + (InternalGrid::right_down::value << 1)))
};

template<typename Dimensions, typename Layout>
BitBoard const Board<Dimensions, Layout>::QUAD_NEAREST_NEIGHBOR_MAGIC =
        DOUBLE_NEAREST_NEIGHBOR_MAGIC[0] ^ DOUBLE_NEAREST_NEIGHBOR_MAGIC[1];

}       // namespace board
}       // namespace dctl
