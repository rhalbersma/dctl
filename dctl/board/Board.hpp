#pragma once
#include <boost/mpl/arithmetic.hpp>             // plus
#include <boost/mpl/int.hpp>                    // int_
#include <boost/preprocessor/repetition.hpp>    // BOOST_PP_ENUM
#include <dctl/board/Grid.hpp>
#include <dctl/board/MetaTemplates.hpp>
#include <dctl/board/Shift.hpp>
#include <dctl/board/Structure.hpp>
#include <dctl/angle/Degrees.hpp>
#include <dctl/bit/Bit.hpp>
#include <dctl/mpl/transform.hpp>
#include <dctl/node/Side.hpp>
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {
namespace board {

template
<
        typename Dimensions,                                    // height, width and parity
        typename Structure = Structure<>                        // dmz, ghosts and internal rotation
>
struct Board
:
        public Dimensions,
        public Structure
{
public:
        // external and internal grids
        typedef Grid<Dimensions> ExternalGrid;
        typedef Grid<typename
                mpl::rotate<
                        Dimensions, typename
                        Structure::full_angle
                >::type, typename
                Structure::ghosts
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
                init_jump_start< Board, mpl::rotate< Direction, typename Structure::full_angle > >
        {};

        static bool is_valid(int square)
        {
                return (begin() <= square && square < end());
        }

        static int begin()
        {
                return (0);
        }

        static int end()
        {
                return (ExternalGrid::size::value);
        }

        static int square2bit(int square)
        {
                return (SQUARE2BIT[square]);
        }

        static int bit2square(int b)
        {
                return (BIT2SQUARE[b]);
        }

        // essential bitboard masks
        static BitBoard const squares;                          // bit mask of legal squares, excluding borders
        static BitBoard const initial_mask[];                   // initial position
        static BitBoard const promotion_mask[][2];              // promotion zones
        static BitBoard const row_mask[][12];                   // bit masks for the rows
        static BitBoard const col_mask[][12];                   // bit masks for the columns

        // detaililiary bitboard masks
        static BitBoard const QUAD_NEAREST_NEIGHBOR_MAGIC;      // shifting bits in 4 directions
        static BitBoard const DOUBLE_NEAREST_NEIGHBOR_MAGIC[];  // shifting bits in 2 directions
        static BitBoard const jump_start[];                     // squares from which a jump is possible in a direction

private:
        // square to bit and bit to square conversion tables
        static int const SQUARE2BIT[];                          // convert a square to a bit
        static int const BIT2SQUARE[];                          // convert a bit to a square
};

template<typename Dimensions, typename Structure>
BitBoard const Board<Dimensions, Structure>::squares = init_squares<Board>::value;

template<typename Dimensions, typename Structure>
BitBoard const Board<Dimensions, Structure>::initial_mask[] = {
        init_initial< Board, Side::black >::value,
        init_initial< Board, Side::white >::value
};

#define DCTL_PP_ROW_MASK(z, i, data)      \
        init_row_mask<Board, data, i>::value

template<typename Dimensions, typename Structure>
BitBoard const Board<Dimensions, Structure>::promotion_mask[][2] = {
        { BOOST_PP_ENUM(2, DCTL_PP_ROW_MASK, Side::white) },
        { BOOST_PP_ENUM(2, DCTL_PP_ROW_MASK, Side::black) }
};

template<typename Dimensions, typename Structure>
BitBoard const Board<Dimensions, Structure>::row_mask[][12] = {
        { BOOST_PP_ENUM(12, DCTL_PP_ROW_MASK, Side::black) },
        { BOOST_PP_ENUM(12, DCTL_PP_ROW_MASK, Side::white) }
};

#undef DCTL_PP_ROW_MASK

#define DCTL_PP_COL_MASK(z, i, data)      \
        init_col_mask<Board, data, i>::value

template<typename Dimensions, typename Structure>
BitBoard const Board<Dimensions, Structure>::col_mask[][12] = {
        { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::black) },
        { BOOST_PP_ENUM(12, DCTL_PP_COL_MASK, Side::white) }
};

#undef DCTL_PP_COL_MASK

template<typename Dimensions, typename Structure>
BitBoard const Board<Dimensions, Structure>::DOUBLE_NEAREST_NEIGHBOR_MAGIC[] = {
        (bit::singlet<BitBoard>(1)) ^ (bit::singlet<BitBoard>(1 + (InternalGrid::left_down::value  << 1))),
        (bit::singlet<BitBoard>(0)) ^ (bit::singlet<BitBoard>(0 + (InternalGrid::right_down::value << 1)))
};

template<typename Dimensions, typename Structure>
BitBoard const Board<Dimensions, Structure>::QUAD_NEAREST_NEIGHBOR_MAGIC =
        DOUBLE_NEAREST_NEIGHBOR_MAGIC[0] ^ DOUBLE_NEAREST_NEIGHBOR_MAGIC[1];

#define DCTL_PP_SQUARE2BIT(z, i, data) \
        square_to_bit< Board, boost::mpl::int_<i> >::type::number::value

template<typename Dimensions, typename Structure>
int const Board<Dimensions, Structure>::SQUARE2BIT[] = {
        BOOST_PP_ENUM(64, DCTL_PP_SQUARE2BIT, ~)
};

#undef DCTL_PP_SQUARE2BIT

#define DCTL_PP_BIT2SQUARE(z, i, data) \
        bit_to_square< Board, boost::mpl::int_<i> >::type::number::value

template<typename Dimensions, typename Structure>
int const Board<Dimensions, Structure>::BIT2SQUARE[] = {
        BOOST_PP_ENUM(64, DCTL_PP_BIT2SQUARE, ~)
};

#undef DCTL_PP_BIT2SQUARE

}       // namespace board

// TODO: use C++11 template aliases
template<typename Board, typename Direction>
struct shift_size
:
        Board::template shift_size<Direction>
{};

// TODO: use C+11 template aliases
template<typename Board, typename Direction>
struct jump_start
:
        Board::template jump_start<Direction>
{};

}       // namespace dctl
