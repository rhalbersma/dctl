#include "Direction.h"
#include "Coordinates.h"

namespace board {

template<typename T, int SQ>
class IS_VALID
{
public:
        static const bool VALUE = SQ < T::SIZE;
};

template<typename T, bool C, int SQ>
class IS_INITIAL
{
private:
        enum {
                ROW_MIN = C? (T::HEIGHT - 1) - ((T::HEIGHT - T::DMZ) / 2 - 1) : 0,
                ROW_MAX = C? (T::HEIGHT - 1) : (T::HEIGHT - T::DMZ) / 2 - 1,
                ROW = Square2Coordiates< Square<T, SQ> >::type::row
        };

public:
        static const bool VALUE = (ROW >= ROW_MIN) && (ROW <= ROW_MAX);
};

template<typename T, bool C, int ROW, int SQ>
struct IS_ROW_MASK
{
        static const bool VALUE = Square2Coordinates< Square<T, SQ> >::type::row == (C? (T::HEIGHT - 1) - ROW : ROW);
};

template<typename T, bool C, int COL, int SQ>
struct IS_COL_MASK
{
        static const bool VALUE = Square2Coordinates< Square<T, SQ> >::type::col == (C? (T::WIDTH - 1) - COL : COL);
};

template<typename T, int FROM, int DEST>
class IS_MAN_JUMP_GROUP
{
private: 
        enum {
                FROM_ROW = Square2Coordinates< Square<T, FROM> >::type::row,
                DEST_ROW = Square2Coordinates< Square<T, DEST> >::type::row,
                FROM_COL = Square2Coordinates< Square<T, FROM> >::type::col,
                DEST_COL = Square2Coordinates< Square<T, DEST> >::type::col,
                R1 = (FROM_ROW - DEST_ROW) % 4,
                C1 = (FROM_COL - DEST_COL) % 4,
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

template<typename T, int I, int SQ>
class IS_JUMPABLE
{
private:
        enum {
                OFFSET = Traits<I>::IS_DIAGONAL? 2 : 4,
                ROW_MIN = Traits<I>::IS_UP? OFFSET : 0,
                ROW_MAX = (T::HEIGHT - 1) - (Traits<I>::IS_DOWN? OFFSET : 0),
                COL_MIN = Traits<I>::IS_LEFT? OFFSET : 0,
                COL_MAX = (T::WIDTH - 1) - (Traits<I>::IS_RIGHT? OFFSET : 0),
                ROW = Square2Coordinates< Square<T, SQ> >::type::row,
                COL = Square2Coordinates< Square<T, SQ> >::type::col
        };

public:
        // a jump in direction <I> is possible if square <SQ> is within OFFSET of the edges being approached
        static const bool VALUE =
	        (ROW >= ROW_MIN) &&
		(ROW <= ROW_MAX) &&
		(COL >= COL_MIN) &&
		(COL <= COL_MAX)
	;
};

}       // namespace board
