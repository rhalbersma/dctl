#include "Direction.h"
#include "BordersPredicates.h"
#include "SquaresPredicates.h"

namespace Geometry {

template<typename T, bool C, size_t SQ>
class IS_INITIAL
{
private:
        enum {
                LOWER_BOUND = C? (T::HEIGHT - 1) - ((T::HEIGHT - T::DMZ) / 2 - 1) : 0,
                UPPER_BOUND = C? (T::HEIGHT - 1) : (T::HEIGHT - T::DMZ) / 2 - 1,
                ROW = SQUARE2COORD<T, SQ>::ROW
        };

public:
        static const bool VALUE = (ROW >= LOWER_BOUND) && (ROW <= UPPER_BOUND);
};

template<typename T, bool C, size_t ROW, size_t SQ>
struct IS_ROW_MASK
{
        static const bool VALUE = SQUARE2COORD<T, SQ>::ROW == (C? (T::HEIGHT - 1) - ROW : ROW);
};

template<typename T, bool C, size_t COL, size_t SQ>
struct IS_COL_MASK
{
        static const bool VALUE = SQUARE2COORD<T, SQ>::COL == (C? (T::WIDTH - 1) - COL : COL);
};

template<typename T, size_t FROM, size_t DEST>
class IS_MAN_JUMP_GROUP
{
private: 
        enum {
                R1 = (SQUARE2COORD<T, FROM>::ROW - SQUARE2COORD<T, DEST>::ROW) % 4,
                C1 = (SQUARE2COORD<T, FROM>::COL - SQUARE2COORD<T, DEST>::COL) % 4,
                R2 = (R1 + 2) % 4,
                C2 = (C1 + 2) % 4
        };

public:
        // a diagonal or orthogonal man capture between square <FROM> and square <DEST> is possible if 
        static const bool VALUE =
        	(!R1 && !C1) || // row AND column numbers difference == 0 mod 4 (even number of captures)
                (!R2 && !C2)    // row AND column numbers difference == 2 mod 4 (odd number of captures)
        ;
};

template<typename T, size_t I, size_t SQ>
class IS_JUMPABLE
{
private:
        enum {
                OFFSET = DirTraits<I>::IS_DIAGONAL? 2 : 4,
                ROW_LOWER_BOUND = DirTraits<I>::IS_UP? OFFSET : 0,
                ROW_UPPER_BOUND = (T::HEIGHT - 1) - (DirTraits<I>::IS_DOWN? OFFSET : 0),
                COL_LOWER_BOUND = DirTraits<I>::IS_LEFT? OFFSET : 0,
                COL_UPPER_BOUND = (T::WIDTH - 1) - (DirTraits<I>::IS_RIGHT? OFFSET : 0),
                ROW = SQUARE2COORD<T, SQ>::ROW,
                COL = SQUARE2COORD<T, SQ>::COL
        };

public:
        // a jump in direction <I> is possible if square <SQ> is within OFFSET of the edges being approached
        static const bool VALUE =
	        (ROW >= ROW_LOWER_BOUND) &&
		(ROW <= ROW_UPPER_BOUND) &&
		(COL >= COL_LOWER_BOUND) &&
		(COL <= COL_UPPER_BOUND)
	;
};

}       // namespace Geometry
