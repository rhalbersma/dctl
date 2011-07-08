#include "BoardMetaTemplates.h"
#include "../Tree/Node/Side.h"
#include "../Utilities/TemplateTricks.h"

namespace board {

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::SQUARES = init_squares<T>::value;

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::INITIAL[] = {
	init_initial<T, tree::node::Side::BLACK>::value,
        init_initial<T, tree::node::Side::WHITE>::value
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::PROMOTION[][2] = {
        {
                init_row_mask<T, tree::node::Side::WHITE, 0>::value,
                init_row_mask<T, tree::node::Side::WHITE, 1>::value
        },
        {
                init_row_mask<T, tree::node::Side::BLACK, 0>::value,
                init_row_mask<T, tree::node::Side::BLACK, 1>::value
        }
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::ROW_MASK[][12] = {
        {
                init_row_mask<T, tree::node::Side::BLACK,  0>::value,
                init_row_mask<T, tree::node::Side::BLACK,  1>::value,
                init_row_mask<T, tree::node::Side::BLACK,  2>::value,
                init_row_mask<T, tree::node::Side::BLACK,  3>::value,
                init_row_mask<T, tree::node::Side::BLACK,  4>::value,
                init_row_mask<T, tree::node::Side::BLACK,  5>::value,
                init_row_mask<T, tree::node::Side::BLACK,  6>::value,
                init_row_mask<T, tree::node::Side::BLACK,  7>::value,
                init_row_mask<T, tree::node::Side::BLACK,  8>::value,
                init_row_mask<T, tree::node::Side::BLACK,  9>::value,
                init_row_mask<T, tree::node::Side::BLACK, 10>::value,
                init_row_mask<T, tree::node::Side::BLACK, 11>::value,
        },
        {
                init_row_mask<T, tree::node::Side::WHITE,  0>::value,
                init_row_mask<T, tree::node::Side::WHITE,  1>::value,
                init_row_mask<T, tree::node::Side::WHITE,  2>::value,
                init_row_mask<T, tree::node::Side::WHITE,  3>::value,
                init_row_mask<T, tree::node::Side::WHITE,  4>::value,
                init_row_mask<T, tree::node::Side::WHITE,  5>::value,
                init_row_mask<T, tree::node::Side::WHITE,  6>::value,
                init_row_mask<T, tree::node::Side::WHITE,  7>::value,
                init_row_mask<T, tree::node::Side::WHITE,  8>::value,
                init_row_mask<T, tree::node::Side::WHITE,  9>::value,
                init_row_mask<T, tree::node::Side::WHITE, 10>::value,
                init_row_mask<T, tree::node::Side::WHITE, 11>::value,
        }
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::COL_MASK[][12] = {
        {
                init_col_mask<T, tree::node::Side::BLACK,  0>::value,
                init_col_mask<T, tree::node::Side::BLACK,  1>::value,
                init_col_mask<T, tree::node::Side::BLACK,  2>::value,
                init_col_mask<T, tree::node::Side::BLACK,  3>::value,
                init_col_mask<T, tree::node::Side::BLACK,  4>::value,
                init_col_mask<T, tree::node::Side::BLACK,  5>::value,
                init_col_mask<T, tree::node::Side::BLACK,  6>::value,
                init_col_mask<T, tree::node::Side::BLACK,  7>::value,
                init_col_mask<T, tree::node::Side::BLACK,  8>::value,
                init_col_mask<T, tree::node::Side::BLACK,  9>::value,
                init_col_mask<T, tree::node::Side::BLACK, 10>::value,
                init_col_mask<T, tree::node::Side::BLACK, 11>::value,
        },
        {
                init_col_mask<T, tree::node::Side::WHITE,  0>::value,
                init_col_mask<T, tree::node::Side::WHITE,  1>::value,
                init_col_mask<T, tree::node::Side::WHITE,  2>::value,
                init_col_mask<T, tree::node::Side::WHITE,  3>::value,
                init_col_mask<T, tree::node::Side::WHITE,  4>::value,
                init_col_mask<T, tree::node::Side::WHITE,  5>::value,
                init_col_mask<T, tree::node::Side::WHITE,  6>::value,
                init_col_mask<T, tree::node::Side::WHITE,  7>::value,
                init_col_mask<T, tree::node::Side::WHITE,  8>::value,
                init_col_mask<T, tree::node::Side::WHITE,  9>::value,
                init_col_mask<T, tree::node::Side::WHITE, 10>::value,
                init_col_mask<T, tree::node::Side::WHITE, 11>::value,
        }
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::DOUBLE_NEAREST_NEIGHBOR_MAGIC[] = {
        (BitBoard(1) << 1) ^ (BitBoard(1) << (1 + (InternalGrid::LEFT_DOWN  << 1))),
        (BitBoard(1) << 0) ^ (BitBoard(1) << (0 + (InternalGrid::RIGHT_DOWN << 1)))
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::QUAD_NEAREST_NEIGHBOR_MAGIC =
        DOUBLE_NEAREST_NEIGHBOR_MAGIC[0] ^ DOUBLE_NEAREST_NEIGHBOR_MAGIC[1];

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::MAN_JUMP_GROUP[] = {
        init_man_jump_group<T, InternalGrid::EDGE_LE + 0>::value,
        init_man_jump_group<T, InternalGrid::EDGE_LE + 1>::value,
        init_man_jump_group<T, InternalGrid::EDGE_LO + 0>::value,
        init_man_jump_group<T, InternalGrid::EDGE_LO + 1>::value
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::JUMPABLE[] = {
        init_jumpable<T, Rotate<Int2Type<Angle::D000>, ANGLE>::value>::value,
        init_jumpable<T, Rotate<Int2Type<Angle::D045>, ANGLE>::value>::value,
        init_jumpable<T, Rotate<Int2Type<Angle::D090>, ANGLE>::value>::value,
        init_jumpable<T, Rotate<Int2Type<Angle::D135>, ANGLE>::value>::value,
        init_jumpable<T, Rotate<Int2Type<Angle::D180>, ANGLE>::value>::value,
        init_jumpable<T, Rotate<Int2Type<Angle::D225>, ANGLE>::value>::value,
        init_jumpable<T, Rotate<Int2Type<Angle::D270>, ANGLE>::value>::value,
        init_jumpable<T, Rotate<Int2Type<Angle::D315>, ANGLE>::value>::value
};

template<typename GridLayout, int D, int N, int A>
const int Board<GridLayout, D, N, A>::DIRECTION[] = {
        InternalGrid::RIGHT, 
        InternalGrid::RIGHT_UP, 
        InternalGrid::UP, 
        InternalGrid::LEFT_UP, 
        InternalGrid::LEFT, 
        InternalGrid::LEFT_DOWN, 
        InternalGrid::DOWN, 
        InternalGrid::RIGHT_DOWN
};

template<typename GridLayout, int D, int N, int A>
const int Board<GridLayout, D, N, A>::TABLE_SQUARE2BIT[] = {
        square2bit<T,  0>::value, square2bit<T,  1>::value, square2bit<T,  2>::value, square2bit<T,  3>::value,
        square2bit<T,  4>::value, square2bit<T,  5>::value, square2bit<T,  6>::value, square2bit<T,  7>::value,
        square2bit<T,  8>::value, square2bit<T,  9>::value, square2bit<T, 10>::value, square2bit<T, 11>::value,
        square2bit<T, 12>::value, square2bit<T, 13>::value, square2bit<T, 14>::value, square2bit<T, 15>::value,
        square2bit<T, 16>::value, square2bit<T, 17>::value, square2bit<T, 18>::value, square2bit<T, 19>::value,
        square2bit<T, 20>::value, square2bit<T, 21>::value, square2bit<T, 22>::value, square2bit<T, 23>::value,
        square2bit<T, 24>::value, square2bit<T, 25>::value, square2bit<T, 26>::value, square2bit<T, 27>::value,
        square2bit<T, 28>::value, square2bit<T, 29>::value, square2bit<T, 30>::value, square2bit<T, 31>::value,
        square2bit<T, 32>::value, square2bit<T, 33>::value, square2bit<T, 34>::value, square2bit<T, 35>::value,
        square2bit<T, 36>::value, square2bit<T, 37>::value, square2bit<T, 38>::value, square2bit<T, 39>::value,
        square2bit<T, 40>::value, square2bit<T, 41>::value, square2bit<T, 42>::value, square2bit<T, 43>::value,
        square2bit<T, 44>::value, square2bit<T, 45>::value, square2bit<T, 46>::value, square2bit<T, 47>::value,
        square2bit<T, 48>::value, square2bit<T, 49>::value, square2bit<T, 50>::value, square2bit<T, 51>::value,
        square2bit<T, 52>::value, square2bit<T, 53>::value, square2bit<T, 54>::value, square2bit<T, 55>::value,
        square2bit<T, 56>::value, square2bit<T, 57>::value, square2bit<T, 58>::value, square2bit<T, 59>::value,
        square2bit<T, 60>::value, square2bit<T, 61>::value, square2bit<T, 62>::value, square2bit<T, 63>::value
};

template<typename GridLayout, int D, int N, int A>
const int Board<GridLayout, D, N, A>::TABLE_BIT2SQUARE[] = {
        bit2square<T,  0>::value, bit2square<T,  1>::value, bit2square<T,  2>::value, bit2square<T,  3>::value,
        bit2square<T,  4>::value, bit2square<T,  5>::value, bit2square<T,  6>::value, bit2square<T,  7>::value,
        bit2square<T,  8>::value, bit2square<T,  9>::value, bit2square<T, 10>::value, bit2square<T, 11>::value,
        bit2square<T, 12>::value, bit2square<T, 13>::value, bit2square<T, 14>::value, bit2square<T, 15>::value,
        bit2square<T, 16>::value, bit2square<T, 17>::value, bit2square<T, 18>::value, bit2square<T, 19>::value,
        bit2square<T, 20>::value, bit2square<T, 21>::value, bit2square<T, 22>::value, bit2square<T, 23>::value,
        bit2square<T, 24>::value, bit2square<T, 25>::value, bit2square<T, 26>::value, bit2square<T, 27>::value,
        bit2square<T, 28>::value, bit2square<T, 29>::value, bit2square<T, 30>::value, bit2square<T, 31>::value,
        bit2square<T, 32>::value, bit2square<T, 33>::value, bit2square<T, 34>::value, bit2square<T, 35>::value,
        bit2square<T, 36>::value, bit2square<T, 37>::value, bit2square<T, 38>::value, bit2square<T, 39>::value,
        bit2square<T, 40>::value, bit2square<T, 41>::value, bit2square<T, 42>::value, bit2square<T, 43>::value,
        bit2square<T, 44>::value, bit2square<T, 45>::value, bit2square<T, 46>::value, bit2square<T, 47>::value,
        bit2square<T, 48>::value, bit2square<T, 49>::value, bit2square<T, 50>::value, bit2square<T, 51>::value,
        bit2square<T, 52>::value, bit2square<T, 53>::value, bit2square<T, 54>::value, bit2square<T, 55>::value,
        bit2square<T, 56>::value, bit2square<T, 57>::value, bit2square<T, 58>::value, bit2square<T, 59>::value,
        bit2square<T, 60>::value, bit2square<T, 61>::value, bit2square<T, 62>::value, bit2square<T, 63>::value
};

}       // namespace board
