#include "BoardPredicates.h"

template<typename T, size_t B>
class INIT_GHOSTS
{
private:
        static const BitBoard MASK = BIT_IS_GHOST<T, B>::VALUE? (BitBoard(1) << B) : 0;

public:
        static const BitBoard VALUE = MASK ^ INIT_GHOSTS<T, B-1>::VALUE;
};

template<typename T>
class INIT_GHOSTS<T, 0>
{
private:
        static const BitBoard MASK = BIT_IS_GHOST<T, 0>::VALUE? (BitBoard(1) << 0) : 0;

public:
        static const BitBoard VALUE = MASK;
};

template<typename T, size_t SQ>
class INIT_SQUARES
{
private:
        static const BitBoard MASK = SQUARE_IS_VALID<T, SQ>::VALUE? (BitBoard(1) << SQUARE2BIT<T, SQ>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK ^ INIT_SQUARES<T, SQ-1>::VALUE;
};

template<typename T>
class INIT_SQUARES<T, 0>
{
private:
        static const BitBoard MASK = SQUARE_IS_VALID<T, 0>::VALUE? (BitBoard(1) << SQUARE2BIT<T, 0>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK;
};

template<typename T, bool C, size_t SQ>
class INIT_INITIAL
{
private:
        static const BitBoard MASK = IS_INITIAL<T, C, SQ>::VALUE? (BitBoard(1) << SQUARE2BIT<T, SQ>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK ^ INIT_INITIAL<T, C, SQ-1>::VALUE;
};

template<typename T, bool C>
class INIT_INITIAL<T, C, 0>
{
private:
        static const BitBoard MASK = IS_INITIAL<T, C, 0>::VALUE? (BitBoard(1) << SQUARE2BIT<T, 0>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK;
};

template<typename T, bool C, size_t ROW, size_t SQ>
class INIT_ROW_MASK
{
private:
        static const BitBoard MASK = IS_ROW_MASK<T, C, ROW, SQ>::VALUE? (BitBoard(1) << SQUARE2BIT<T, SQ>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK ^ INIT_ROW_MASK<T, C, ROW, SQ-1>::VALUE;
};

template<typename T, bool C, size_t ROW>
class INIT_ROW_MASK<T, C, ROW, 0>
{
private:
        static const BitBoard MASK = IS_ROW_MASK<T, C, ROW, 0>::VALUE? (BitBoard(1) << SQUARE2BIT<T, 0>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK;
};

template<typename T, bool C, size_t COL, size_t SQ>
class INIT_COL_MASK
{
private:
        static const BitBoard MASK = IS_COL_MASK<T, C, COL, SQ>::VALUE? (BitBoard(1) << SQUARE2BIT<T, SQ>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK ^ INIT_COL_MASK<T, C, COL, SQ-1>::VALUE;
};

template<typename T, bool C, size_t COL>
class INIT_COL_MASK<T, C, COL, 0>
{
private:
        static const BitBoard MASK = IS_COL_MASK<T, C, COL, 0>::VALUE? (BitBoard(1) << SQUARE2BIT<T, 0>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK;
};

template<typename T, size_t G, size_t SQ>
class INIT_MAN_JUMP_GROUP
{
private:
        static const BitBoard MASK = IS_MAN_JUMP_GROUP<T, G, SQ>::VALUE? (BitBoard(1) << SQUARE2BIT<T, SQ>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK ^ INIT_MAN_JUMP_GROUP<T, G, SQ-1>::VALUE;
};

template<typename T, size_t G>
class INIT_MAN_JUMP_GROUP<T, G, 0>
{
private:
        static const BitBoard MASK = IS_MAN_JUMP_GROUP<T, G, 0>::VALUE? (BitBoard(1) << SQUARE2BIT<T, 0>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK;
};

template<typename T, size_t I, size_t SQ>
class INIT_JUMPABLE
{
private:
        static const BitBoard MASK = IS_JUMPABLE<T, I, SQ>::VALUE? (BitBoard(1) << SQUARE2BIT<T, SQ>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK ^ INIT_JUMPABLE<T, I, SQ-1>::VALUE;
};

template<typename T, size_t I>
class INIT_JUMPABLE<T, I, 0>
{
private:
        static const BitBoard MASK = IS_JUMPABLE<T, I, 0>::VALUE? (BitBoard(1) << SQUARE2BIT<T, 0>::VALUE) : 0;

public:
        static const BitBoard VALUE = MASK;
};

template<typename T, size_t SQ>
class SQUARE2BIT
{
private:
        typedef typename T::ExternalGrid E;
        typedef typename T::InternalGrid I;

        enum {
                ROW = SQUARE2COORD<E, SQ>::ROW,
                COL = SQUARE2COORD<E, SQ>::COL,
                ROW_PRIME = RotateCoordinate<E, ROW, COL, T::ANGLE_S2B>::ROW,
                COL_PRIME = RotateCoordinate<E, ROW, COL, T::ANGLE_S2B>::COL
        };

public:
        enum { VALUE = COORD2BIT<typename T::G, ROW_PRIME, COL_PRIME>::VALUE };
};

template<typename T, size_t B>
class BIT2SQUARE
{
private:
        typedef typename T::ExternalGrid E;
        typedef typename T::InternalGrid I;

        enum {
                ROW = BIT2COORD<T::G, B>::ROW,
                COL = BIT2COORD<T::G, B>::COL,
                ROW_PRIME = RotateCoordinate<I, ROW, COL, T::ANGLE_B2S>::ROW,
                COL_PRIME = RotateCoordinate<I, ROW, COL, T::ANGLE_B2S>::COL,
                INVALID = 999
        };

public:
        enum { VALUE = BIT_IS_GHOST<typename T::G, B>::VALUE? INVALID : COORD2SQUARE<E, ROW_PRIME, COL_PRIME>::VALUE };
};
