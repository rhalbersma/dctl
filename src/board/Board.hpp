#include "BoardMetaTemplates.h"
#include "../Tree/Node/Side.h"

namespace board {

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::SQUARES = INIT_SQUARES<T>::VALUE;

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::INITIAL[] = {
	INIT_INITIAL<T, tree::node::Side::BLACK>::VALUE,
        INIT_INITIAL<T, tree::node::Side::WHITE>::VALUE
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::PROMOTION[][2] = {
        {
                INIT_ROW_MASK<T, tree::node::Side::WHITE, 0>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE, 1>::VALUE
        },
        {
                INIT_ROW_MASK<T, tree::node::Side::BLACK, 0>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK, 1>::VALUE
        }
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::ROW_MASK[][12] = {
        {
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  0>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  1>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  2>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  3>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  4>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  5>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  6>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  7>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  8>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK,  9>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK, 10>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::BLACK, 11>::VALUE,
        },
        {
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  0>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  1>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  2>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  3>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  4>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  5>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  6>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  7>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  8>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE,  9>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE, 10>::VALUE,
                INIT_ROW_MASK<T, tree::node::Side::WHITE, 11>::VALUE,
        }
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::COL_MASK[][12] = {
        {
                INIT_COL_MASK<T, tree::node::Side::BLACK,  0>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK,  1>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK,  2>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK,  3>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK,  4>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK,  5>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK,  6>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK,  7>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK,  8>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK,  9>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK, 10>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::BLACK, 11>::VALUE,
        },
        {
                INIT_COL_MASK<T, tree::node::Side::WHITE,  0>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE,  1>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE,  2>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE,  3>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE,  4>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE,  5>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE,  6>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE,  7>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE,  8>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE,  9>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE, 10>::VALUE,
                INIT_COL_MASK<T, tree::node::Side::WHITE, 11>::VALUE,
        }
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::DOUBLE_NEAREST_NEIGHBOR_MAGIC[] = {
        (BitBoard(1) << 1) ^ (BitBoard(1) << (1 + (BordersGrid::LEFT_DOWN  << 1))),
        (BitBoard(1) << 0) ^ (BitBoard(1) << (0 + (BordersGrid::RIGHT_DOWN << 1)))
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::QUAD_NEAREST_NEIGHBOR_MAGIC =
        DOUBLE_NEAREST_NEIGHBOR_MAGIC[0] ^ DOUBLE_NEAREST_NEIGHBOR_MAGIC[1];

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::MAN_JUMP_GROUP[] = {
        INIT_MAN_JUMP_GROUP<T, BordersGrid::EDGE_LE + 0>::VALUE,
        INIT_MAN_JUMP_GROUP<T, BordersGrid::EDGE_LE + 1>::VALUE,
        INIT_MAN_JUMP_GROUP<T, BordersGrid::EDGE_LO + 0>::VALUE,
        INIT_MAN_JUMP_GROUP<T, BordersGrid::EDGE_LO + 1>::VALUE
};

template<typename GridLayout, int D, int N, int A>
const BitBoard Board<GridLayout, D, N, A>::JUMPABLE[] = {
        INIT_JUMPABLE<T, angles::Rotate<angles::D000, ANGLE>::VALUE>::VALUE,
        INIT_JUMPABLE<T, angles::Rotate<angles::D045, ANGLE>::VALUE>::VALUE,
        INIT_JUMPABLE<T, angles::Rotate<angles::D090, ANGLE>::VALUE>::VALUE,
        INIT_JUMPABLE<T, angles::Rotate<angles::D135, ANGLE>::VALUE>::VALUE,
        INIT_JUMPABLE<T, angles::Rotate<angles::D180, ANGLE>::VALUE>::VALUE,
        INIT_JUMPABLE<T, angles::Rotate<angles::D225, ANGLE>::VALUE>::VALUE,
        INIT_JUMPABLE<T, angles::Rotate<angles::D270, ANGLE>::VALUE>::VALUE,
        INIT_JUMPABLE<T, angles::Rotate<angles::D315, ANGLE>::VALUE>::VALUE
};

template<typename GridLayout, int D, int N, int A>
const int Board<GridLayout, D, N, A>::DIRECTION[] = {
        BordersGrid::RIGHT, 
        BordersGrid::RIGHT_UP, 
        BordersGrid::UP, 
        BordersGrid::LEFT_UP, 
        BordersGrid::LEFT, 
        BordersGrid::LEFT_DOWN, 
        BordersGrid::DOWN, 
        BordersGrid::RIGHT_DOWN
};

template<typename GridLayout, int D, int N, int A>
const int Board<GridLayout, D, N, A>::TABLE_SQUARE2BIT[] = {
        SQUARE2BIT<T,  0>::VALUE, SQUARE2BIT<T,  1>::VALUE, SQUARE2BIT<T,  2>::VALUE, SQUARE2BIT<T,  3>::VALUE,
        SQUARE2BIT<T,  4>::VALUE, SQUARE2BIT<T,  5>::VALUE, SQUARE2BIT<T,  6>::VALUE, SQUARE2BIT<T,  7>::VALUE,
        SQUARE2BIT<T,  8>::VALUE, SQUARE2BIT<T,  9>::VALUE, SQUARE2BIT<T, 10>::VALUE, SQUARE2BIT<T, 11>::VALUE,
        SQUARE2BIT<T, 12>::VALUE, SQUARE2BIT<T, 13>::VALUE, SQUARE2BIT<T, 14>::VALUE, SQUARE2BIT<T, 15>::VALUE,
        SQUARE2BIT<T, 16>::VALUE, SQUARE2BIT<T, 17>::VALUE, SQUARE2BIT<T, 18>::VALUE, SQUARE2BIT<T, 19>::VALUE,
        SQUARE2BIT<T, 20>::VALUE, SQUARE2BIT<T, 21>::VALUE, SQUARE2BIT<T, 22>::VALUE, SQUARE2BIT<T, 23>::VALUE,
        SQUARE2BIT<T, 24>::VALUE, SQUARE2BIT<T, 25>::VALUE, SQUARE2BIT<T, 26>::VALUE, SQUARE2BIT<T, 27>::VALUE,
        SQUARE2BIT<T, 28>::VALUE, SQUARE2BIT<T, 29>::VALUE, SQUARE2BIT<T, 30>::VALUE, SQUARE2BIT<T, 31>::VALUE,
        SQUARE2BIT<T, 32>::VALUE, SQUARE2BIT<T, 33>::VALUE, SQUARE2BIT<T, 34>::VALUE, SQUARE2BIT<T, 35>::VALUE,
        SQUARE2BIT<T, 36>::VALUE, SQUARE2BIT<T, 37>::VALUE, SQUARE2BIT<T, 38>::VALUE, SQUARE2BIT<T, 39>::VALUE,
        SQUARE2BIT<T, 40>::VALUE, SQUARE2BIT<T, 41>::VALUE, SQUARE2BIT<T, 42>::VALUE, SQUARE2BIT<T, 43>::VALUE,
        SQUARE2BIT<T, 44>::VALUE, SQUARE2BIT<T, 45>::VALUE, SQUARE2BIT<T, 46>::VALUE, SQUARE2BIT<T, 47>::VALUE,
        SQUARE2BIT<T, 48>::VALUE, SQUARE2BIT<T, 49>::VALUE, SQUARE2BIT<T, 50>::VALUE, SQUARE2BIT<T, 51>::VALUE,
        SQUARE2BIT<T, 52>::VALUE, SQUARE2BIT<T, 53>::VALUE, SQUARE2BIT<T, 54>::VALUE, SQUARE2BIT<T, 55>::VALUE,
        SQUARE2BIT<T, 56>::VALUE, SQUARE2BIT<T, 57>::VALUE, SQUARE2BIT<T, 58>::VALUE, SQUARE2BIT<T, 59>::VALUE,
        SQUARE2BIT<T, 60>::VALUE, SQUARE2BIT<T, 61>::VALUE, SQUARE2BIT<T, 62>::VALUE, SQUARE2BIT<T, 63>::VALUE
};

template<typename GridLayout, int D, int N, int A>
const int Board<GridLayout, D, N, A>::TABLE_BIT2SQUARE[] = {
        BIT2SQUARE<T,  0>::VALUE, BIT2SQUARE<T,  1>::VALUE, BIT2SQUARE<T,  2>::VALUE, BIT2SQUARE<T,  3>::VALUE,
        BIT2SQUARE<T,  4>::VALUE, BIT2SQUARE<T,  5>::VALUE, BIT2SQUARE<T,  6>::VALUE, BIT2SQUARE<T,  7>::VALUE,
        BIT2SQUARE<T,  8>::VALUE, BIT2SQUARE<T,  9>::VALUE, BIT2SQUARE<T, 10>::VALUE, BIT2SQUARE<T, 11>::VALUE,
        BIT2SQUARE<T, 12>::VALUE, BIT2SQUARE<T, 13>::VALUE, BIT2SQUARE<T, 14>::VALUE, BIT2SQUARE<T, 15>::VALUE,
        BIT2SQUARE<T, 16>::VALUE, BIT2SQUARE<T, 17>::VALUE, BIT2SQUARE<T, 18>::VALUE, BIT2SQUARE<T, 19>::VALUE,
        BIT2SQUARE<T, 20>::VALUE, BIT2SQUARE<T, 21>::VALUE, BIT2SQUARE<T, 22>::VALUE, BIT2SQUARE<T, 23>::VALUE,
        BIT2SQUARE<T, 24>::VALUE, BIT2SQUARE<T, 25>::VALUE, BIT2SQUARE<T, 26>::VALUE, BIT2SQUARE<T, 27>::VALUE,
        BIT2SQUARE<T, 28>::VALUE, BIT2SQUARE<T, 29>::VALUE, BIT2SQUARE<T, 30>::VALUE, BIT2SQUARE<T, 31>::VALUE,
        BIT2SQUARE<T, 32>::VALUE, BIT2SQUARE<T, 33>::VALUE, BIT2SQUARE<T, 34>::VALUE, BIT2SQUARE<T, 35>::VALUE,
        BIT2SQUARE<T, 36>::VALUE, BIT2SQUARE<T, 37>::VALUE, BIT2SQUARE<T, 38>::VALUE, BIT2SQUARE<T, 39>::VALUE,
        BIT2SQUARE<T, 40>::VALUE, BIT2SQUARE<T, 41>::VALUE, BIT2SQUARE<T, 42>::VALUE, BIT2SQUARE<T, 43>::VALUE,
        BIT2SQUARE<T, 44>::VALUE, BIT2SQUARE<T, 45>::VALUE, BIT2SQUARE<T, 46>::VALUE, BIT2SQUARE<T, 47>::VALUE,
        BIT2SQUARE<T, 48>::VALUE, BIT2SQUARE<T, 49>::VALUE, BIT2SQUARE<T, 50>::VALUE, BIT2SQUARE<T, 51>::VALUE,
        BIT2SQUARE<T, 52>::VALUE, BIT2SQUARE<T, 53>::VALUE, BIT2SQUARE<T, 54>::VALUE, BIT2SQUARE<T, 55>::VALUE,
        BIT2SQUARE<T, 56>::VALUE, BIT2SQUARE<T, 57>::VALUE, BIT2SQUARE<T, 58>::VALUE, BIT2SQUARE<T, 59>::VALUE,
        BIT2SQUARE<T, 60>::VALUE, BIT2SQUARE<T, 61>::VALUE, BIT2SQUARE<T, 62>::VALUE, BIT2SQUARE<T, 63>::VALUE
};

}       // namespace board