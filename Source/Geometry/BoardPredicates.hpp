#include "Direction.h"
#include "Coordinates.h"

namespace geometry {

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
                ROW = coordinates::FromRange<T, SQ>::Out::ROW
        };

public:
        static const bool VALUE = (ROW >= ROW_MIN) && (ROW <= ROW_MAX);
};

template<typename T, bool C, int ROW, int SQ>
struct IS_ROW_MASK
{
        static const bool VALUE = coordinates::FromRange<T, SQ>::Out::ROW == (C? (T::HEIGHT - 1) - ROW : ROW);
};

template<typename T, bool C, int COL, int SQ>
struct IS_COL_MASK
{
        static const bool VALUE = coordinates::FromRange<T, SQ>::Out::COL == (C? (T::WIDTH - 1) - COL : COL);
};

template<typename T, int FROM, int DEST>
class IS_MAN_JUMP_GROUP
{
private: 
        enum {
                R1 = (coordinates::FromRange<T, FROM>::Out::ROW - coordinates::FromRange<T, DEST>::Out::ROW) % 4,
                C1 = (coordinates::FromRange<T, FROM>::Out::COL - coordinates::FromRange<T, DEST>::Out::COL) % 4,
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
                OFFSET = direction::Traits<I>::IS_DIAGONAL? 2 : 4,
                ROW_MIN = direction::Traits<I>::IS_UP? OFFSET : 0,
                ROW_MAX = (T::HEIGHT - 1) - (direction::Traits<I>::IS_DOWN? OFFSET : 0),
                COL_MIN = direction::Traits<I>::IS_LEFT? OFFSET : 0,
                COL_MAX = (T::WIDTH - 1) - (direction::Traits<I>::IS_RIGHT? OFFSET : 0),
                ROW = coordinates::FromRange<T, SQ>::Out::ROW,
                COL = coordinates::FromRange<T, SQ>::Out::COL
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

}       // namespace geometry
