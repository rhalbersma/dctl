#include "MetaTemplates.hpp"
#include "../node/Side.hpp"
#include "../utility/TemplateTricks.h"

namespace dctl {
namespace board {

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::squares = init_squares<B>::value;

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::INITIAL[] = {
	init_initial<B, Side::black>::value,
        init_initial<B, Side::white>::value
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
        (BitBoard(1) << 1) ^ (BitBoard(1) << (1 + (InternalGrid::left_down  << 1))),
        (BitBoard(1) << 0) ^ (BitBoard(1) << (0 + (InternalGrid::right_down << 1)))
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::QUAD_NEAREST_NEIGHBOR_MAGIC =
        DOUBLE_NEAREST_NEIGHBOR_MAGIC[0] ^ DOUBLE_NEAREST_NEIGHBOR_MAGIC[1];

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::jump_group[] = {
        init_jump_group<B, InternalGrid::edge_le + 0>::value,
        init_jump_group<B, InternalGrid::edge_le + 1>::value,
        init_jump_group<B, InternalGrid::edge_lo + 0>::value,
        init_jump_group<B, InternalGrid::edge_lo + 1>::value
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::jump_start[] = {
        init_jump_start<B, rotate<Angle<Degrees::D000>, B::angle>::type::index>::value,
        init_jump_start<B, rotate<Angle<Degrees::D045>, B::angle>::type::index>::value,
        init_jump_start<B, rotate<Angle<Degrees::D090>, B::angle>::type::index>::value,
        init_jump_start<B, rotate<Angle<Degrees::D135>, B::angle>::type::index>::value,
        init_jump_start<B, rotate<Angle<Degrees::D180>, B::angle>::type::index>::value,
        init_jump_start<B, rotate<Angle<Degrees::D225>, B::angle>::type::index>::value,
        init_jump_start<B, rotate<Angle<Degrees::D270>, B::angle>::type::index>::value,
        init_jump_start<B, rotate<Angle<Degrees::D315>, B::angle>::type::index>::value
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
        square_to_bit<B,  0>::value, square_to_bit<B,  1>::value, square_to_bit<B,  2>::value, square_to_bit<B,  3>::value,
        square_to_bit<B,  4>::value, square_to_bit<B,  5>::value, square_to_bit<B,  6>::value, square_to_bit<B,  7>::value,
        square_to_bit<B,  8>::value, square_to_bit<B,  9>::value, square_to_bit<B, 10>::value, square_to_bit<B, 11>::value,
        square_to_bit<B, 12>::value, square_to_bit<B, 13>::value, square_to_bit<B, 14>::value, square_to_bit<B, 15>::value,
        square_to_bit<B, 16>::value, square_to_bit<B, 17>::value, square_to_bit<B, 18>::value, square_to_bit<B, 19>::value,
        square_to_bit<B, 20>::value, square_to_bit<B, 21>::value, square_to_bit<B, 22>::value, square_to_bit<B, 23>::value,
        square_to_bit<B, 24>::value, square_to_bit<B, 25>::value, square_to_bit<B, 26>::value, square_to_bit<B, 27>::value,
        square_to_bit<B, 28>::value, square_to_bit<B, 29>::value, square_to_bit<B, 30>::value, square_to_bit<B, 31>::value,
        square_to_bit<B, 32>::value, square_to_bit<B, 33>::value, square_to_bit<B, 34>::value, square_to_bit<B, 35>::value,
        square_to_bit<B, 36>::value, square_to_bit<B, 37>::value, square_to_bit<B, 38>::value, square_to_bit<B, 39>::value,
        square_to_bit<B, 40>::value, square_to_bit<B, 41>::value, square_to_bit<B, 42>::value, square_to_bit<B, 43>::value,
        square_to_bit<B, 44>::value, square_to_bit<B, 45>::value, square_to_bit<B, 46>::value, square_to_bit<B, 47>::value,
        square_to_bit<B, 48>::value, square_to_bit<B, 49>::value, square_to_bit<B, 50>::value, square_to_bit<B, 51>::value,
        square_to_bit<B, 52>::value, square_to_bit<B, 53>::value, square_to_bit<B, 54>::value, square_to_bit<B, 55>::value,
        square_to_bit<B, 56>::value, square_to_bit<B, 57>::value, square_to_bit<B, 58>::value, square_to_bit<B, 59>::value,
        square_to_bit<B, 60>::value, square_to_bit<B, 61>::value, square_to_bit<B, 62>::value, square_to_bit<B, 63>::value
};

template<typename Dimensions, typename Structure>
const int Board<Dimensions, Structure>::BIT2SQUARE[] = {
        bit_to_square<B,  0>::value, bit_to_square<B,  1>::value, bit_to_square<B,  2>::value, bit_to_square<B,  3>::value,
        bit_to_square<B,  4>::value, bit_to_square<B,  5>::value, bit_to_square<B,  6>::value, bit_to_square<B,  7>::value,
        bit_to_square<B,  8>::value, bit_to_square<B,  9>::value, bit_to_square<B, 10>::value, bit_to_square<B, 11>::value,
        bit_to_square<B, 12>::value, bit_to_square<B, 13>::value, bit_to_square<B, 14>::value, bit_to_square<B, 15>::value,
        bit_to_square<B, 16>::value, bit_to_square<B, 17>::value, bit_to_square<B, 18>::value, bit_to_square<B, 19>::value,
        bit_to_square<B, 20>::value, bit_to_square<B, 21>::value, bit_to_square<B, 22>::value, bit_to_square<B, 23>::value,
        bit_to_square<B, 24>::value, bit_to_square<B, 25>::value, bit_to_square<B, 26>::value, bit_to_square<B, 27>::value,
        bit_to_square<B, 28>::value, bit_to_square<B, 29>::value, bit_to_square<B, 30>::value, bit_to_square<B, 31>::value,
        bit_to_square<B, 32>::value, bit_to_square<B, 33>::value, bit_to_square<B, 34>::value, bit_to_square<B, 35>::value,
        bit_to_square<B, 36>::value, bit_to_square<B, 37>::value, bit_to_square<B, 38>::value, bit_to_square<B, 39>::value,
        bit_to_square<B, 40>::value, bit_to_square<B, 41>::value, bit_to_square<B, 42>::value, bit_to_square<B, 43>::value,
        bit_to_square<B, 44>::value, bit_to_square<B, 45>::value, bit_to_square<B, 46>::value, bit_to_square<B, 47>::value,
        bit_to_square<B, 48>::value, bit_to_square<B, 49>::value, bit_to_square<B, 50>::value, bit_to_square<B, 51>::value,
        bit_to_square<B, 52>::value, bit_to_square<B, 53>::value, bit_to_square<B, 54>::value, bit_to_square<B, 55>::value,
        bit_to_square<B, 56>::value, bit_to_square<B, 57>::value, bit_to_square<B, 58>::value, bit_to_square<B, 59>::value,
        bit_to_square<B, 60>::value, bit_to_square<B, 61>::value, bit_to_square<B, 62>::value, bit_to_square<B, 63>::value
};

}       // namespace board
}       // namespace dctl
