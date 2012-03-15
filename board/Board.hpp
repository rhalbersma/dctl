#pragma once
#include "Grid.hpp"
#include "MetaTemplates.hpp"
#include "Structure.hpp"
#include "Transform.hpp"
#include "../node/Side.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace board {

template
<
        typename Dimensions,                                    // height, width and parity
        typename Structure = Structure<>                        // dmz, ghosts and internal rotation
>
class Board
:
        public Dimensions,
        public Structure
{
public:
        // reflection on template parameters
        typedef Board<Dimensions, Structure> B;

        // external and internal grids
        typedef Grid<Dimensions> ExternalGrid;
        typedef Grid<typename rotate<Dimensions, typename Structure::full_angle >::type, Structure::ghosts> InternalGrid;

        // essential bitboard masks
        static const BitBoard squares;                          // bit mask of legal squares, excluding borders
        static const BitBoard INITIAL[];                        // initial position
        static const BitBoard PROMOTION[][2];                   // promotion zones
        static const BitBoard row_mask[][12];                   // bit masks for the rows
        static const BitBoard col_mask[][12];                   // bit masks for the columns

        // auxiliary bitboard masks
        static const BitBoard QUAD_NEAREST_NEIGHBOR_MAGIC;      // shifting bits in 4 directions
        static const BitBoard DOUBLE_NEAREST_NEIGHBOR_MAGIC[];  // shifting bits in 2 directions
        static const BitBoard jump_group[];                     // families of squares reachable by jumping pawns
        static const BitBoard jump_start[];                     // squares from which a jump is possible in a direction

        // arrays of directions
        static const int SHIFT[];                               // the bitwise shifts for all 8 directions

        static bool is_valid(int);
        static int begin();
        static int end();
        static int square2bit(int);
        static int bit2square(int);

private:
        // square to bit and bit to square conversion tables
        static const int SQUARE2BIT[];                          // convert a square to a bit
        static const int BIT2SQUARE[];                          // convert a bit to a square
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::squares = init_squares<B>::value;

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::INITIAL[] = {
        init_initial< B, Side::black >::value,
        init_initial< B, Side::white >::value
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::PROMOTION[][2] = {
        {
                init_row_mask<B, Side::white, 0>::value,
                init_row_mask<B, Side::white, 1>::value
        },
        {
                init_row_mask<B, Side::black, 0>::value,
                init_row_mask<B, Side::black, 1>::value
        }
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::row_mask[][12] = {
        {
                init_row_mask<B, Side::black,  0>::value,
                init_row_mask<B, Side::black,  1>::value,
                init_row_mask<B, Side::black,  2>::value,
                init_row_mask<B, Side::black,  3>::value,
                init_row_mask<B, Side::black,  4>::value,
                init_row_mask<B, Side::black,  5>::value,
                init_row_mask<B, Side::black,  6>::value,
                init_row_mask<B, Side::black,  7>::value,
                init_row_mask<B, Side::black,  8>::value,
                init_row_mask<B, Side::black,  9>::value,
                init_row_mask<B, Side::black, 10>::value,
                init_row_mask<B, Side::black, 11>::value,
        },
        {
                init_row_mask<B, Side::white,  0>::value,
                init_row_mask<B, Side::white,  1>::value,
                init_row_mask<B, Side::white,  2>::value,
                init_row_mask<B, Side::white,  3>::value,
                init_row_mask<B, Side::white,  4>::value,
                init_row_mask<B, Side::white,  5>::value,
                init_row_mask<B, Side::white,  6>::value,
                init_row_mask<B, Side::white,  7>::value,
                init_row_mask<B, Side::white,  8>::value,
                init_row_mask<B, Side::white,  9>::value,
                init_row_mask<B, Side::white, 10>::value,
                init_row_mask<B, Side::white, 11>::value,
        }
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::col_mask[][12] = {
        {
                init_col_mask<B, Side::black,  0>::value,
                init_col_mask<B, Side::black,  1>::value,
                init_col_mask<B, Side::black,  2>::value,
                init_col_mask<B, Side::black,  3>::value,
                init_col_mask<B, Side::black,  4>::value,
                init_col_mask<B, Side::black,  5>::value,
                init_col_mask<B, Side::black,  6>::value,
                init_col_mask<B, Side::black,  7>::value,
                init_col_mask<B, Side::black,  8>::value,
                init_col_mask<B, Side::black,  9>::value,
                init_col_mask<B, Side::black, 10>::value,
                init_col_mask<B, Side::black, 11>::value,
        },
        {
                init_col_mask<B, Side::white,  0>::value,
                init_col_mask<B, Side::white,  1>::value,
                init_col_mask<B, Side::white,  2>::value,
                init_col_mask<B, Side::white,  3>::value,
                init_col_mask<B, Side::white,  4>::value,
                init_col_mask<B, Side::white,  5>::value,
                init_col_mask<B, Side::white,  6>::value,
                init_col_mask<B, Side::white,  7>::value,
                init_col_mask<B, Side::white,  8>::value,
                init_col_mask<B, Side::white,  9>::value,
                init_col_mask<B, Side::white, 10>::value,
                init_col_mask<B, Side::white, 11>::value,
        }
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::DOUBLE_NEAREST_NEIGHBOR_MAGIC[] = {
        (bit::singlet<BitBoard>(1)) ^ (bit::singlet<BitBoard>(1 + (InternalGrid::left_down  << 1))),
        (bit::singlet<BitBoard>(0)) ^ (bit::singlet<BitBoard>(0 + (InternalGrid::right_down << 1)))
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::QUAD_NEAREST_NEIGHBOR_MAGIC =
        DOUBLE_NEAREST_NEIGHBOR_MAGIC[0] ^ DOUBLE_NEAREST_NEIGHBOR_MAGIC[1];

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::jump_group[] = {
        init_jump_group< B, InternalGrid::edge_le + 0 >::value,
        init_jump_group< B, InternalGrid::edge_le + 1 >::value,
        init_jump_group< B, InternalGrid::edge_lo + 0 >::value,
        init_jump_group< B, InternalGrid::edge_lo + 1 >::value
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::jump_start[] = {
        init_jump_start< B, rotate< angle<degrees::D000>, B::full_angle > >::value,
        init_jump_start< B, rotate< angle<degrees::D045>, B::full_angle > >::value,
        init_jump_start< B, rotate< angle<degrees::D090>, B::full_angle > >::value,
        init_jump_start< B, rotate< angle<degrees::D135>, B::full_angle > >::value,
        init_jump_start< B, rotate< angle<degrees::D180>, B::full_angle > >::value,
        init_jump_start< B, rotate< angle<degrees::D225>, B::full_angle > >::value,
        init_jump_start< B, rotate< angle<degrees::D270>, B::full_angle > >::value,
        init_jump_start< B, rotate< angle<degrees::D315>, B::full_angle > >::value
};

template<typename Dimensions, typename Structure>
const int Board<Dimensions, Structure>::SHIFT[] = {
        InternalGrid::right,
        InternalGrid::right_up,
        InternalGrid::up,
        InternalGrid::left_up,
        InternalGrid::left,
        InternalGrid::left_down,
        InternalGrid::down,
        InternalGrid::right_down
};

template<typename Dimensions, typename Structure>
bool Board<Dimensions, Structure>::is_valid(int sq)
{
        return sq >= begin() && sq < end();
}

template<typename Dimensions, typename Structure>
int Board<Dimensions, Structure>::begin()
{
        return 0;
}

template<typename Dimensions, typename Structure>
int Board<Dimensions, Structure>::end()
{
        return ExternalGrid::size;
}

template<typename Dimensions, typename Structure>
int Board<Dimensions, Structure>::square2bit(int sq)
{
        return SQUARE2BIT[sq];
}

template<typename Dimensions, typename Structure>
int Board<Dimensions, Structure>::bit2square(int b)
{
        return BIT2SQUARE[b];
}

template<typename Dimensions, typename Structure>
const int Board<Dimensions, Structure>::SQUARE2BIT[] = {
        square_to_bit<B,  0>::type::value, square_to_bit<B,  1>::type::value, square_to_bit<B,  2>::type::value, square_to_bit<B,  3>::type::value,
        square_to_bit<B,  4>::type::value, square_to_bit<B,  5>::type::value, square_to_bit<B,  6>::type::value, square_to_bit<B,  7>::type::value,
        square_to_bit<B,  8>::type::value, square_to_bit<B,  9>::type::value, square_to_bit<B, 10>::type::value, square_to_bit<B, 11>::type::value,
        square_to_bit<B, 12>::type::value, square_to_bit<B, 13>::type::value, square_to_bit<B, 14>::type::value, square_to_bit<B, 15>::type::value,
        square_to_bit<B, 16>::type::value, square_to_bit<B, 17>::type::value, square_to_bit<B, 18>::type::value, square_to_bit<B, 19>::type::value,
        square_to_bit<B, 20>::type::value, square_to_bit<B, 21>::type::value, square_to_bit<B, 22>::type::value, square_to_bit<B, 23>::type::value,
        square_to_bit<B, 24>::type::value, square_to_bit<B, 25>::type::value, square_to_bit<B, 26>::type::value, square_to_bit<B, 27>::type::value,
        square_to_bit<B, 28>::type::value, square_to_bit<B, 29>::type::value, square_to_bit<B, 30>::type::value, square_to_bit<B, 31>::type::value,
        square_to_bit<B, 32>::type::value, square_to_bit<B, 33>::type::value, square_to_bit<B, 34>::type::value, square_to_bit<B, 35>::type::value,
        square_to_bit<B, 36>::type::value, square_to_bit<B, 37>::type::value, square_to_bit<B, 38>::type::value, square_to_bit<B, 39>::type::value,
        square_to_bit<B, 40>::type::value, square_to_bit<B, 41>::type::value, square_to_bit<B, 42>::type::value, square_to_bit<B, 43>::type::value,
        square_to_bit<B, 44>::type::value, square_to_bit<B, 45>::type::value, square_to_bit<B, 46>::type::value, square_to_bit<B, 47>::type::value,
        square_to_bit<B, 48>::type::value, square_to_bit<B, 49>::type::value, square_to_bit<B, 50>::type::value, square_to_bit<B, 51>::type::value,
        square_to_bit<B, 52>::type::value, square_to_bit<B, 53>::type::value, square_to_bit<B, 54>::type::value, square_to_bit<B, 55>::type::value,
        square_to_bit<B, 56>::type::value, square_to_bit<B, 57>::type::value, square_to_bit<B, 58>::type::value, square_to_bit<B, 59>::type::value,
        square_to_bit<B, 60>::type::value, square_to_bit<B, 61>::type::value, square_to_bit<B, 62>::type::value, square_to_bit<B, 63>::type::value
};

template<typename Dimensions, typename Structure>
const int Board<Dimensions, Structure>::BIT2SQUARE[] = {
        bit_to_square<B,  0>::type::value, bit_to_square<B,  1>::type::value, bit_to_square<B,  2>::type::value, bit_to_square<B,  3>::type::value,
        bit_to_square<B,  4>::type::value, bit_to_square<B,  5>::type::value, bit_to_square<B,  6>::type::value, bit_to_square<B,  7>::type::value,
        bit_to_square<B,  8>::type::value, bit_to_square<B,  9>::type::value, bit_to_square<B, 10>::type::value, bit_to_square<B, 11>::type::value,
        bit_to_square<B, 12>::type::value, bit_to_square<B, 13>::type::value, bit_to_square<B, 14>::type::value, bit_to_square<B, 15>::type::value,
        bit_to_square<B, 16>::type::value, bit_to_square<B, 17>::type::value, bit_to_square<B, 18>::type::value, bit_to_square<B, 19>::type::value,
        bit_to_square<B, 20>::type::value, bit_to_square<B, 21>::type::value, bit_to_square<B, 22>::type::value, bit_to_square<B, 23>::type::value,
        bit_to_square<B, 24>::type::value, bit_to_square<B, 25>::type::value, bit_to_square<B, 26>::type::value, bit_to_square<B, 27>::type::value,
        bit_to_square<B, 28>::type::value, bit_to_square<B, 29>::type::value, bit_to_square<B, 30>::type::value, bit_to_square<B, 31>::type::value,
        bit_to_square<B, 32>::type::value, bit_to_square<B, 33>::type::value, bit_to_square<B, 34>::type::value, bit_to_square<B, 35>::type::value,
        bit_to_square<B, 36>::type::value, bit_to_square<B, 37>::type::value, bit_to_square<B, 38>::type::value, bit_to_square<B, 39>::type::value,
        bit_to_square<B, 40>::type::value, bit_to_square<B, 41>::type::value, bit_to_square<B, 42>::type::value, bit_to_square<B, 43>::type::value,
        bit_to_square<B, 44>::type::value, bit_to_square<B, 45>::type::value, bit_to_square<B, 46>::type::value, bit_to_square<B, 47>::type::value,
        bit_to_square<B, 48>::type::value, bit_to_square<B, 49>::type::value, bit_to_square<B, 50>::type::value, bit_to_square<B, 51>::type::value,
        bit_to_square<B, 52>::type::value, bit_to_square<B, 53>::type::value, bit_to_square<B, 54>::type::value, bit_to_square<B, 55>::type::value,
        bit_to_square<B, 56>::type::value, bit_to_square<B, 57>::type::value, bit_to_square<B, 58>::type::value, bit_to_square<B, 59>::type::value,
        bit_to_square<B, 60>::type::value, bit_to_square<B, 61>::type::value, bit_to_square<B, 62>::type::value, bit_to_square<B, 63>::type::value
};

}       // namespace board
}       // namespace dctl
