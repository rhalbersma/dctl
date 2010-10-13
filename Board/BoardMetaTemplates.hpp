// template meta programs
#include "BoardPredicates.h"

template<typename Board, size_t B>
struct INIT_GHOSTS
{
        static const BitBoard VALUE = (IS_GHOST_BIT<Board, B>::VALUE? (BitBoard(1) << B) : 0) ^ INIT_GHOSTS<Board, B-1>::VALUE;
};

template<typename Board>
struct INIT_GHOSTS<Board, 0>
{
        static const BitBoard VALUE = IS_GHOST_BIT<Board, 0>::VALUE? (BitBoard(1) << 0) : 0;
};

template<typename Board, bool C, size_t B>
struct INIT_INITIAL
{
        static const BitBoard VALUE = ((IS_INITIAL<Board, C, B>::VALUE && !IS_GHOST_BIT<Board, B>::VALUE)? (BitBoard(1) << B) : 0) ^ INIT_INITIAL<Board, C, B-1>::VALUE;
};

template<typename Board, bool C>
struct INIT_INITIAL<Board, C, 0>
{
        static const BitBoard VALUE = (IS_INITIAL<Board, C, 0>::VALUE && !IS_GHOST_BIT<Board, 0>::VALUE)? (BitBoard(1) << 0) : 0;
};

template<typename Board, bool Color, size_t R, size_t B>
struct INIT_ROW_MASK
{
        static const BitBoard VALUE = ((IS_ROW_MASK<Board, Color, R, B>::VALUE && !IS_GHOST_BIT<Board, B>::VALUE)? (BitBoard(1) << B) : 0) ^ INIT_ROW_MASK<Board, Color, R, B-1>::VALUE;
};

template<typename Board, bool Color, size_t R>
struct INIT_ROW_MASK<Board, Color, R, 0>
{
        static const BitBoard VALUE = (IS_ROW_MASK<Board, Color, R, 0>::VALUE && !IS_GHOST_BIT<Board, 0>::VALUE)? (BitBoard(1) << 0) : 0;
};

template<typename Board, bool Color, size_t C, size_t B>
struct INIT_COL_MASK
{
        static const BitBoard VALUE = ((IS_COL_MASK<Board, Color, C, B>::VALUE && !IS_GHOST_BIT<Board, B>::VALUE)? (BitBoard(1) << B) : 0) ^ INIT_COL_MASK<Board, Color, C, B-1>::VALUE;
};

template<typename Board, bool Color, size_t C>
struct INIT_COL_MASK<Board, Color, C, 0>
{
        static const BitBoard VALUE = (IS_COL_MASK<Board, Color, C, 0>::VALUE && !IS_GHOST_BIT<Board, 0>::VALUE)? (BitBoard(1) << 0) : 0;
};

template<typename Board, size_t G, size_t B>
struct INIT_MAN_JUMP_GROUP
{
        static const BitBoard VALUE = ((IS_MAN_JUMP_GROUP<Board, G, B>::VALUE && !IS_GHOST_BIT<Board, B>::VALUE)? (BitBoard(1) << B) : 0) ^ INIT_MAN_JUMP_GROUP<Board, G, B-1>::VALUE;
};

template<typename Board, size_t G>
struct INIT_MAN_JUMP_GROUP<Board, G, 0>
{
        static const BitBoard VALUE = (IS_MAN_JUMP_GROUP<Board, G, 0>::VALUE && !IS_GHOST_BIT<Board, 0>::VALUE)? (BitBoard(1) << 0) : 0;
};

template<typename Board, size_t I, size_t B>
struct INIT_JUMPABLE
{
        static const BitBoard VALUE = ((IS_JUMPABLE<Board, I, B>::VALUE && !IS_GHOST_BIT<Board, B>::VALUE)? (BitBoard(1) << B) : 0) ^ INIT_JUMPABLE<Board, I, B-1>::VALUE;
};

template<typename Board, size_t I>
struct INIT_JUMPABLE<Board, I, 0>
{
        static const BitBoard VALUE = (IS_JUMPABLE<Board, I, 0>::VALUE && !IS_GHOST_BIT<Board, 0>::VALUE)? (BitBoard(1) << 0) : 0;
};

enum { INVALID = 999 };

template<typename Board, size_t B>
struct BIT2SQUARE
{
        static const size_t VALUE = (Board::MIN_BITS <= B|| IS_GHOST_BIT<Board, B>::VALUE)? INVALID : PREV_SQUARE<Board, B>::VALUE + 1;
};

template<typename Board>
struct BIT2SQUARE<Board, 0>
{
	static const size_t VALUE = 0;
};

template<typename Board, size_t SQ>
struct SQUARE2BIT
{
        static const size_t VALUE = (Board::NUM_SQUARES <= SQ)? INVALID : PREV_BIT<Board, SQ-1, Board::NUM_BITS-1>::VALUE;
};

template<typename Board>
struct SQUARE2BIT<Board, 0>
{
        static const size_t VALUE = 0;
};

template<typename Board, size_t B>
struct PREV_SQUARE
{
        static const size_t VALUE = IS_GHOST_BIT<Board, B-1>::VALUE? PREV_SQUARE<Board, B-1>::VALUE : BIT2SQUARE<Board, B-1>::VALUE;
};

template<typename Board>
struct PREV_SQUARE<Board, 0>
{
	static const size_t VALUE = INVALID;
};

template<typename Board, size_t SQ, size_t B>
struct PREV_BIT
{
	static const size_t VALUE = (PREV_SQUARE<Board, B>::VALUE == SQ)? B : PREV_BIT<Board, SQ, B-1>::VALUE;
};

template<typename Board, size_t SQ>
struct PREV_BIT<Board, SQ, 0>
{
	static const size_t VALUE = INVALID;
};
