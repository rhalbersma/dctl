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
        // external and internal grids
        typedef Grid<Dimensions> ExternalGrid;
        typedef Grid<typename 
                rotate<
                        Dimensions, typename 
                        Structure::full_angle
                >::type, 
                Structure::ghosts
        > InternalGrid;

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
const BitBoard Board<Dimensions, Structure>::squares = init_squares<Board>::value;

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::INITIAL[] = {
        init_initial< Board, Side::black >::value,
        init_initial< Board, Side::white >::value
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::PROMOTION[][2] = {
        {
                init_row_mask<Board, Side::white, 0>::value,
                init_row_mask<Board, Side::white, 1>::value
        },
        {
                init_row_mask<Board, Side::black, 0>::value,
                init_row_mask<Board, Side::black, 1>::value
        }
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::row_mask[][12] = {
        {
                init_row_mask<Board, Side::black,  0>::value,
                init_row_mask<Board, Side::black,  1>::value,
                init_row_mask<Board, Side::black,  2>::value,
                init_row_mask<Board, Side::black,  3>::value,
                init_row_mask<Board, Side::black,  4>::value,
                init_row_mask<Board, Side::black,  5>::value,
                init_row_mask<Board, Side::black,  6>::value,
                init_row_mask<Board, Side::black,  7>::value,
                init_row_mask<Board, Side::black,  8>::value,
                init_row_mask<Board, Side::black,  9>::value,
                init_row_mask<Board, Side::black, 10>::value,
                init_row_mask<Board, Side::black, 11>::value,
        },
        {
                init_row_mask<Board, Side::white,  0>::value,
                init_row_mask<Board, Side::white,  1>::value,
                init_row_mask<Board, Side::white,  2>::value,
                init_row_mask<Board, Side::white,  3>::value,
                init_row_mask<Board, Side::white,  4>::value,
                init_row_mask<Board, Side::white,  5>::value,
                init_row_mask<Board, Side::white,  6>::value,
                init_row_mask<Board, Side::white,  7>::value,
                init_row_mask<Board, Side::white,  8>::value,
                init_row_mask<Board, Side::white,  9>::value,
                init_row_mask<Board, Side::white, 10>::value,
                init_row_mask<Board, Side::white, 11>::value,
        }
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::col_mask[][12] = {
        {
                init_col_mask<Board, Side::black,  0>::value,
                init_col_mask<Board, Side::black,  1>::value,
                init_col_mask<Board, Side::black,  2>::value,
                init_col_mask<Board, Side::black,  3>::value,
                init_col_mask<Board, Side::black,  4>::value,
                init_col_mask<Board, Side::black,  5>::value,
                init_col_mask<Board, Side::black,  6>::value,
                init_col_mask<Board, Side::black,  7>::value,
                init_col_mask<Board, Side::black,  8>::value,
                init_col_mask<Board, Side::black,  9>::value,
                init_col_mask<Board, Side::black, 10>::value,
                init_col_mask<Board, Side::black, 11>::value,
        },
        {
                init_col_mask<Board, Side::white,  0>::value,
                init_col_mask<Board, Side::white,  1>::value,
                init_col_mask<Board, Side::white,  2>::value,
                init_col_mask<Board, Side::white,  3>::value,
                init_col_mask<Board, Side::white,  4>::value,
                init_col_mask<Board, Side::white,  5>::value,
                init_col_mask<Board, Side::white,  6>::value,
                init_col_mask<Board, Side::white,  7>::value,
                init_col_mask<Board, Side::white,  8>::value,
                init_col_mask<Board, Side::white,  9>::value,
                init_col_mask<Board, Side::white, 10>::value,
                init_col_mask<Board, Side::white, 11>::value,
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
        init_jump_group< Board, InternalGrid::edge_le + 0 >::value,
        init_jump_group< Board, InternalGrid::edge_le + 1 >::value,
        init_jump_group< Board, InternalGrid::edge_lo + 0 >::value,
        init_jump_group< Board, InternalGrid::edge_lo + 1 >::value
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::jump_start[] = {
        init_jump_start< Board, rotate< angle<degrees::D000>, Board::full_angle > >::value,
        init_jump_start< Board, rotate< angle<degrees::D045>, Board::full_angle > >::value,
        init_jump_start< Board, rotate< angle<degrees::D090>, Board::full_angle > >::value,
        init_jump_start< Board, rotate< angle<degrees::D135>, Board::full_angle > >::value,
        init_jump_start< Board, rotate< angle<degrees::D180>, Board::full_angle > >::value,
        init_jump_start< Board, rotate< angle<degrees::D225>, Board::full_angle > >::value,
        init_jump_start< Board, rotate< angle<degrees::D270>, Board::full_angle > >::value,
        init_jump_start< Board, rotate< angle<degrees::D315>, Board::full_angle > >::value
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
        square_to_bit<Board,  0>::type::value, square_to_bit<Board,  1>::type::value, square_to_bit<Board,  2>::type::value, square_to_bit<Board,  3>::type::value,
        square_to_bit<Board,  4>::type::value, square_to_bit<Board,  5>::type::value, square_to_bit<Board,  6>::type::value, square_to_bit<Board,  7>::type::value,
        square_to_bit<Board,  8>::type::value, square_to_bit<Board,  9>::type::value, square_to_bit<Board, 10>::type::value, square_to_bit<Board, 11>::type::value,
        square_to_bit<Board, 12>::type::value, square_to_bit<Board, 13>::type::value, square_to_bit<Board, 14>::type::value, square_to_bit<Board, 15>::type::value,
        square_to_bit<Board, 16>::type::value, square_to_bit<Board, 17>::type::value, square_to_bit<Board, 18>::type::value, square_to_bit<Board, 19>::type::value,
        square_to_bit<Board, 20>::type::value, square_to_bit<Board, 21>::type::value, square_to_bit<Board, 22>::type::value, square_to_bit<Board, 23>::type::value,
        square_to_bit<Board, 24>::type::value, square_to_bit<Board, 25>::type::value, square_to_bit<Board, 26>::type::value, square_to_bit<Board, 27>::type::value,
        square_to_bit<Board, 28>::type::value, square_to_bit<Board, 29>::type::value, square_to_bit<Board, 30>::type::value, square_to_bit<Board, 31>::type::value,
        square_to_bit<Board, 32>::type::value, square_to_bit<Board, 33>::type::value, square_to_bit<Board, 34>::type::value, square_to_bit<Board, 35>::type::value,
        square_to_bit<Board, 36>::type::value, square_to_bit<Board, 37>::type::value, square_to_bit<Board, 38>::type::value, square_to_bit<Board, 39>::type::value,
        square_to_bit<Board, 40>::type::value, square_to_bit<Board, 41>::type::value, square_to_bit<Board, 42>::type::value, square_to_bit<Board, 43>::type::value,
        square_to_bit<Board, 44>::type::value, square_to_bit<Board, 45>::type::value, square_to_bit<Board, 46>::type::value, square_to_bit<Board, 47>::type::value,
        square_to_bit<Board, 48>::type::value, square_to_bit<Board, 49>::type::value, square_to_bit<Board, 50>::type::value, square_to_bit<Board, 51>::type::value,
        square_to_bit<Board, 52>::type::value, square_to_bit<Board, 53>::type::value, square_to_bit<Board, 54>::type::value, square_to_bit<Board, 55>::type::value,
        square_to_bit<Board, 56>::type::value, square_to_bit<Board, 57>::type::value, square_to_bit<Board, 58>::type::value, square_to_bit<Board, 59>::type::value,
        square_to_bit<Board, 60>::type::value, square_to_bit<Board, 61>::type::value, square_to_bit<Board, 62>::type::value, square_to_bit<Board, 63>::type::value
};

template<typename Dimensions, typename Structure>
const int Board<Dimensions, Structure>::BIT2SQUARE[] = {
        bit_to_square<Board,  0>::type::value, bit_to_square<Board,  1>::type::value, bit_to_square<Board,  2>::type::value, bit_to_square<Board,  3>::type::value,
        bit_to_square<Board,  4>::type::value, bit_to_square<Board,  5>::type::value, bit_to_square<Board,  6>::type::value, bit_to_square<Board,  7>::type::value,
        bit_to_square<Board,  8>::type::value, bit_to_square<Board,  9>::type::value, bit_to_square<Board, 10>::type::value, bit_to_square<Board, 11>::type::value,
        bit_to_square<Board, 12>::type::value, bit_to_square<Board, 13>::type::value, bit_to_square<Board, 14>::type::value, bit_to_square<Board, 15>::type::value,
        bit_to_square<Board, 16>::type::value, bit_to_square<Board, 17>::type::value, bit_to_square<Board, 18>::type::value, bit_to_square<Board, 19>::type::value,
        bit_to_square<Board, 20>::type::value, bit_to_square<Board, 21>::type::value, bit_to_square<Board, 22>::type::value, bit_to_square<Board, 23>::type::value,
        bit_to_square<Board, 24>::type::value, bit_to_square<Board, 25>::type::value, bit_to_square<Board, 26>::type::value, bit_to_square<Board, 27>::type::value,
        bit_to_square<Board, 28>::type::value, bit_to_square<Board, 29>::type::value, bit_to_square<Board, 30>::type::value, bit_to_square<Board, 31>::type::value,
        bit_to_square<Board, 32>::type::value, bit_to_square<Board, 33>::type::value, bit_to_square<Board, 34>::type::value, bit_to_square<Board, 35>::type::value,
        bit_to_square<Board, 36>::type::value, bit_to_square<Board, 37>::type::value, bit_to_square<Board, 38>::type::value, bit_to_square<Board, 39>::type::value,
        bit_to_square<Board, 40>::type::value, bit_to_square<Board, 41>::type::value, bit_to_square<Board, 42>::type::value, bit_to_square<Board, 43>::type::value,
        bit_to_square<Board, 44>::type::value, bit_to_square<Board, 45>::type::value, bit_to_square<Board, 46>::type::value, bit_to_square<Board, 47>::type::value,
        bit_to_square<Board, 48>::type::value, bit_to_square<Board, 49>::type::value, bit_to_square<Board, 50>::type::value, bit_to_square<Board, 51>::type::value,
        bit_to_square<Board, 52>::type::value, bit_to_square<Board, 53>::type::value, bit_to_square<Board, 54>::type::value, bit_to_square<Board, 55>::type::value,
        bit_to_square<Board, 56>::type::value, bit_to_square<Board, 57>::type::value, bit_to_square<Board, 58>::type::value, bit_to_square<Board, 59>::type::value,
        bit_to_square<Board, 60>::type::value, bit_to_square<Board, 61>::type::value, bit_to_square<Board, 62>::type::value, bit_to_square<Board, 63>::type::value
};

}       // namespace board
}       // namespace dctl
