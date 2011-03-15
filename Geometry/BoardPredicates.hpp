#include "Direction.h"
#include "Coordinates.h"

namespace Geometry {

template<typename T, size_t SQ>
class IS_VALID
{
public:
        static const bool VALUE = SQ < T::SIZE;
};

template<typename T, bool C, size_t SQ>
class IS_INITIAL
{
private:
        enum {
                ROW_MIN = C? (T::HEIGHT - 1) - ((T::HEIGHT - T::DMZ) / 2 - 1) : 0,
                ROW_MAX = C? (T::HEIGHT - 1) : (T::HEIGHT - T::DMZ) / 2 - 1,
                ROW = Coordinates::FromRange<T, SQ>::Out::ROW
        };

public:
        static const bool VALUE = (ROW >= ROW_MIN) && (ROW <= ROW_MAX);
};

template<typename T, bool C, size_t ROW, size_t SQ>
struct IS_ROW_MASK
{
        static const bool VALUE = Coordinates::FromRange<T, SQ>::Out::ROW == (C? (T::HEIGHT - 1) - ROW : ROW);
};

template<typename T, bool C, size_t COL, size_t SQ>
struct IS_COL_MASK
{
        static const bool VALUE = Coordinates::FromRange<T, SQ>::Out::COL == (C? (T::WIDTH - 1) - COL : COL);
};

template<typename T, size_t FROM, size_t DEST>
class IS_MAN_JUMP_GROUP
{
private: 
        enum {
                R1 = (Coordinates::FromRange<T, FROM>::Out::ROW - Coordinates::FromRange<T, DEST>::Out::ROW) % 4,
                C1 = (Coordinates::FromRange<T, FROM>::Out::COL - Coordinates::FromRange<T, DEST>::Out::COL) % 4,
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
                OFFSET = Direction::Traits<I>::IS_DIAGONAL? 2 : 4,
                ROW_MIN = Direction::Traits<I>::IS_UP? OFFSET : 0,
                ROW_MAX = (T::HEIGHT - 1) - (Direction::Traits<I>::IS_DOWN? OFFSET : 0),
                COL_MIN = Direction::Traits<I>::IS_LEFT? OFFSET : 0,
                COL_MAX = (T::WIDTH - 1) - (Direction::Traits<I>::IS_RIGHT? OFFSET : 0),
                ROW = Coordinates::FromRange<T, SQ>::Out::ROW,
                COL = Coordinates::FromRange<T, SQ>::Out::COL
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

}       // namespace Geometry
