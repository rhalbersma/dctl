#include "MetaTemplates.h"
#include "../Tree/Node/Side.h"
#include "../Utilities/TemplateTricks.h"

namespace board {

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::SQUARES = init_squares<B>::value;

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::INITIAL[] = {
	init_initial<B, tree::node::Side::BLACK>::value,
        init_initial<B, tree::node::Side::WHITE>::value
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::PROMOTION[][2] = {
        {
                init_row_mask<B, tree::node::Side::WHITE, 0>::value,
                init_row_mask<B, tree::node::Side::WHITE, 1>::value
        },
        {
                init_row_mask<B, tree::node::Side::BLACK, 0>::value,
                init_row_mask<B, tree::node::Side::BLACK, 1>::value
        }
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::ROW_MASK[][12] = {
        {
                init_row_mask<B, tree::node::Side::BLACK,  0>::value,
                init_row_mask<B, tree::node::Side::BLACK,  1>::value,
                init_row_mask<B, tree::node::Side::BLACK,  2>::value,
                init_row_mask<B, tree::node::Side::BLACK,  3>::value,
                init_row_mask<B, tree::node::Side::BLACK,  4>::value,
                init_row_mask<B, tree::node::Side::BLACK,  5>::value,
                init_row_mask<B, tree::node::Side::BLACK,  6>::value,
                init_row_mask<B, tree::node::Side::BLACK,  7>::value,
                init_row_mask<B, tree::node::Side::BLACK,  8>::value,
                init_row_mask<B, tree::node::Side::BLACK,  9>::value,
                init_row_mask<B, tree::node::Side::BLACK, 10>::value,
                init_row_mask<B, tree::node::Side::BLACK, 11>::value,
        },
        {
                init_row_mask<B, tree::node::Side::WHITE,  0>::value,
                init_row_mask<B, tree::node::Side::WHITE,  1>::value,
                init_row_mask<B, tree::node::Side::WHITE,  2>::value,
                init_row_mask<B, tree::node::Side::WHITE,  3>::value,
                init_row_mask<B, tree::node::Side::WHITE,  4>::value,
                init_row_mask<B, tree::node::Side::WHITE,  5>::value,
                init_row_mask<B, tree::node::Side::WHITE,  6>::value,
                init_row_mask<B, tree::node::Side::WHITE,  7>::value,
                init_row_mask<B, tree::node::Side::WHITE,  8>::value,
                init_row_mask<B, tree::node::Side::WHITE,  9>::value,
                init_row_mask<B, tree::node::Side::WHITE, 10>::value,
                init_row_mask<B, tree::node::Side::WHITE, 11>::value,
        }
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::COL_MASK[][12] = {
        {
                init_col_mask<B, tree::node::Side::BLACK,  0>::value,
                init_col_mask<B, tree::node::Side::BLACK,  1>::value,
                init_col_mask<B, tree::node::Side::BLACK,  2>::value,
                init_col_mask<B, tree::node::Side::BLACK,  3>::value,
                init_col_mask<B, tree::node::Side::BLACK,  4>::value,
                init_col_mask<B, tree::node::Side::BLACK,  5>::value,
                init_col_mask<B, tree::node::Side::BLACK,  6>::value,
                init_col_mask<B, tree::node::Side::BLACK,  7>::value,
                init_col_mask<B, tree::node::Side::BLACK,  8>::value,
                init_col_mask<B, tree::node::Side::BLACK,  9>::value,
                init_col_mask<B, tree::node::Side::BLACK, 10>::value,
                init_col_mask<B, tree::node::Side::BLACK, 11>::value,
        },
        {
                init_col_mask<B, tree::node::Side::WHITE,  0>::value,
                init_col_mask<B, tree::node::Side::WHITE,  1>::value,
                init_col_mask<B, tree::node::Side::WHITE,  2>::value,
                init_col_mask<B, tree::node::Side::WHITE,  3>::value,
                init_col_mask<B, tree::node::Side::WHITE,  4>::value,
                init_col_mask<B, tree::node::Side::WHITE,  5>::value,
                init_col_mask<B, tree::node::Side::WHITE,  6>::value,
                init_col_mask<B, tree::node::Side::WHITE,  7>::value,
                init_col_mask<B, tree::node::Side::WHITE,  8>::value,
                init_col_mask<B, tree::node::Side::WHITE,  9>::value,
                init_col_mask<B, tree::node::Side::WHITE, 10>::value,
                init_col_mask<B, tree::node::Side::WHITE, 11>::value,
        }
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::DOUBLE_NEAREST_NEIGHBOR_MAGIC[] = {
        (BitBoard(1) << 1) ^ (BitBoard(1) << (1 + (InternalGrid::LEFT_DOWN  << 1))),
        (BitBoard(1) << 0) ^ (BitBoard(1) << (0 + (InternalGrid::RIGHT_DOWN << 1)))
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::QUAD_NEAREST_NEIGHBOR_MAGIC =
        DOUBLE_NEAREST_NEIGHBOR_MAGIC[0] ^ DOUBLE_NEAREST_NEIGHBOR_MAGIC[1];

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::MAN_JUMP_GROUP[] = {
        init_man_jump_group<B, InternalGrid::EDGE_LE + 0>::value,
        init_man_jump_group<B, InternalGrid::EDGE_LE + 1>::value,
        init_man_jump_group<B, InternalGrid::EDGE_LO + 0>::value,
        init_man_jump_group<B, InternalGrid::EDGE_LO + 1>::value
};

template<typename Dimensions, typename Structure>
const BitBoard Board<Dimensions, Structure>::JUMPABLE[] = {
        init_jumpable<B, Rotate<Int2Type<Angle::D000>, ANGLE>::value>::value,
        init_jumpable<B, Rotate<Int2Type<Angle::D045>, ANGLE>::value>::value,
        init_jumpable<B, Rotate<Int2Type<Angle::D090>, ANGLE>::value>::value,
        init_jumpable<B, Rotate<Int2Type<Angle::D135>, ANGLE>::value>::value,
        init_jumpable<B, Rotate<Int2Type<Angle::D180>, ANGLE>::value>::value,
        init_jumpable<B, Rotate<Int2Type<Angle::D225>, ANGLE>::value>::value,
        init_jumpable<B, Rotate<Int2Type<Angle::D270>, ANGLE>::value>::value,
        init_jumpable<B, Rotate<Int2Type<Angle::D315>, ANGLE>::value>::value
};

template<typename Dimensions, typename Structure>
const int Board<Dimensions, Structure>::DIRECTION[] = {
        InternalGrid::RIGHT,
        InternalGrid::RIGHT_UP,
        InternalGrid::UP,
        InternalGrid::LEFT_UP,
        InternalGrid::LEFT,
        InternalGrid::LEFT_DOWN,
        InternalGrid::DOWN,
        InternalGrid::RIGHT_DOWN
};

template<typename Dimensions, typename Structure>
const int Board<Dimensions, Structure>::TABLE_SQUARE2BIT[] = {
        square2bit<B,  0>::value, square2bit<B,  1>::value, square2bit<B,  2>::value, square2bit<B,  3>::value,
        square2bit<B,  4>::value, square2bit<B,  5>::value, square2bit<B,  6>::value, square2bit<B,  7>::value,
        square2bit<B,  8>::value, square2bit<B,  9>::value, square2bit<B, 10>::value, square2bit<B, 11>::value,
        square2bit<B, 12>::value, square2bit<B, 13>::value, square2bit<B, 14>::value, square2bit<B, 15>::value,
        square2bit<B, 16>::value, square2bit<B, 17>::value, square2bit<B, 18>::value, square2bit<B, 19>::value,
        square2bit<B, 20>::value, square2bit<B, 21>::value, square2bit<B, 22>::value, square2bit<B, 23>::value,
        square2bit<B, 24>::value, square2bit<B, 25>::value, square2bit<B, 26>::value, square2bit<B, 27>::value,
        square2bit<B, 28>::value, square2bit<B, 29>::value, square2bit<B, 30>::value, square2bit<B, 31>::value,
        square2bit<B, 32>::value, square2bit<B, 33>::value, square2bit<B, 34>::value, square2bit<B, 35>::value,
        square2bit<B, 36>::value, square2bit<B, 37>::value, square2bit<B, 38>::value, square2bit<B, 39>::value,
        square2bit<B, 40>::value, square2bit<B, 41>::value, square2bit<B, 42>::value, square2bit<B, 43>::value,
        square2bit<B, 44>::value, square2bit<B, 45>::value, square2bit<B, 46>::value, square2bit<B, 47>::value,
        square2bit<B, 48>::value, square2bit<B, 49>::value, square2bit<B, 50>::value, square2bit<B, 51>::value,
        square2bit<B, 52>::value, square2bit<B, 53>::value, square2bit<B, 54>::value, square2bit<B, 55>::value,
        square2bit<B, 56>::value, square2bit<B, 57>::value, square2bit<B, 58>::value, square2bit<B, 59>::value,
        square2bit<B, 60>::value, square2bit<B, 61>::value, square2bit<B, 62>::value, square2bit<B, 63>::value
};

template<typename Dimensions, typename Structure>
const int Board<Dimensions, Structure>::TABLE_BIT2SQUARE[] = {
        bit2square<B,  0>::value, bit2square<B,  1>::value, bit2square<B,  2>::value, bit2square<B,  3>::value,
        bit2square<B,  4>::value, bit2square<B,  5>::value, bit2square<B,  6>::value, bit2square<B,  7>::value,
        bit2square<B,  8>::value, bit2square<B,  9>::value, bit2square<B, 10>::value, bit2square<B, 11>::value,
        bit2square<B, 12>::value, bit2square<B, 13>::value, bit2square<B, 14>::value, bit2square<B, 15>::value,
        bit2square<B, 16>::value, bit2square<B, 17>::value, bit2square<B, 18>::value, bit2square<B, 19>::value,
        bit2square<B, 20>::value, bit2square<B, 21>::value, bit2square<B, 22>::value, bit2square<B, 23>::value,
        bit2square<B, 24>::value, bit2square<B, 25>::value, bit2square<B, 26>::value, bit2square<B, 27>::value,
        bit2square<B, 28>::value, bit2square<B, 29>::value, bit2square<B, 30>::value, bit2square<B, 31>::value,
        bit2square<B, 32>::value, bit2square<B, 33>::value, bit2square<B, 34>::value, bit2square<B, 35>::value,
        bit2square<B, 36>::value, bit2square<B, 37>::value, bit2square<B, 38>::value, bit2square<B, 39>::value,
        bit2square<B, 40>::value, bit2square<B, 41>::value, bit2square<B, 42>::value, bit2square<B, 43>::value,
        bit2square<B, 44>::value, bit2square<B, 45>::value, bit2square<B, 46>::value, bit2square<B, 47>::value,
        bit2square<B, 48>::value, bit2square<B, 49>::value, bit2square<B, 50>::value, bit2square<B, 51>::value,
        bit2square<B, 52>::value, bit2square<B, 53>::value, bit2square<B, 54>::value, bit2square<B, 55>::value,
        bit2square<B, 56>::value, bit2square<B, 57>::value, bit2square<B, 58>::value, bit2square<B, 59>::value,
        bit2square<B, 60>::value, bit2square<B, 61>::value, bit2square<B, 62>::value, bit2square<B, 63>::value
};

}       // namespace board
