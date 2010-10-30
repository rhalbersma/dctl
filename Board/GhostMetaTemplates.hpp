#include "GhostPredicates.h"
#include "Transform.h"

enum { INVALID = 999 };

template<typename T, size_t B>
class BIT2SQUARE
{
private:
        enum {
                ROW = BIT2COORD<T, B>::ROW,
                COL = BIT2COORD<T, B>::COL,
                ROW_PRIME = RotateCoordinate<T, ROW, COL, T::ANGLE>::ROW,
                COL_PRIME = RotateCoordinate<T, ROW, COL, T::ANGLE>::COL
        };

        typedef typename RotateGrid<T, T::ANGLE>::Out T_PRIME;

public:
        enum { VALUE = BIT_IS_GHOST<T, B>::VALUE? INVALID : COORD2SQUARE<T_PRIME, ROW_PRIME, COL_PRIME>::VALUE };
};

template<typename T, size_t SQ>
class SQUARE2BIT
{
private:
        enum {
                ROW = SQUARE2COORD<T, SQ>::ROW,
                COL = SQUARE2COORD<T, SQ>::COL,
                ROW_PRIME = RotateCoordinate<T, ROW, COL, T::ANGLE>::ROW,
                COL_PRIME = RotateCoordinate<T, ROW, COL, T::ANGLE>::COL
        };

        typedef Ghost<typename RotateGrid<T, T::ANGLE>::Out, T::GLUE, T::ANGLE> T_PRIME;

public:
        enum { VALUE = COORD2BIT<T_PRIME, ROW_PRIME, COL_PRIME>::VALUE };
};
