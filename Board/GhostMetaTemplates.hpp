#include "GhostPredicates.h"

enum { INVALID = 999 };

template<typename T, size_t B>
class BIT2SQUARE
{
private:
        enum {
                ROW = BIT2COORD<T, B>::ROW,
                COL = BIT2COORD<T, B>::COL
        };

public:
        enum { VALUE = BIT_IS_GHOST<T, B>::VALUE? INVALID : COORD2SQUARE<T, ROW, COL>::VALUE };
};

template<typename T, size_t SQ>
class SQUARE2BIT
{
private:
        enum {
                ROW = SQUARE2COORD<T, SQ>::ROW,
                COL = SQUARE2COORD<T, SQ>::COL
        };

public:
        enum { VALUE = COORD2BIT<T, ROW, COL>::VALUE };
};
