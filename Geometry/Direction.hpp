#include "Transform.h"

namespace Geometry {

// the 8 diagonal and orthogonal direction indices for black and white to move
template<typename Board, bool Color> 
class DirIndex
{ 
private:                        
        enum { ANGLE = (D180 * !Color) + Board::A_PRIME };

public:
        enum {
                RIGHT       = AntiClockwise<D000, ANGLE>::VALUE,
                RIGHT_UP    = AntiClockwise<D045, ANGLE>::VALUE,
                UP          = AntiClockwise<D090, ANGLE>::VALUE,
                LEFT_UP     = AntiClockwise<D135, ANGLE>::VALUE,
                LEFT        = AntiClockwise<D180, ANGLE>::VALUE,
                LEFT_DOWN   = AntiClockwise<D225, ANGLE>::VALUE,
                DOWN        = AntiClockwise<D270, ANGLE>::VALUE,
                RIGHT_DOWN  = AntiClockwise<D315, ANGLE>::VALUE
        };
};

template<size_t I>
class DirTraits
{
private:
        static const size_t I_L090 = Rotate<I>::L090;

public:
        // diagonality, orthogonality
        static const bool IS_DIAGONAL = I % 2;                                  // 1, 3, 5, 7
        static const bool IS_ORTHOGONAL = !IS_DIAGONAL;                         // 0, 2, 4, 6

        // up, down, right, left
        static const bool IS_UP = !(I / 4) && (I % 4);                          // 1, 2, 3
        static const bool IS_DOWN = (I / 4) && (I % 4);                         // 5, 6, 7
        static const bool IS_RIGHT = DirTraits<I_L090>::IS_UP;                  // 0, 1, 7
        static const bool IS_LEFT = DirTraits<I_L090>::IS_DOWN;                 // 3, 4, 5

        // positive, negative
        static const bool IS_POSITIVE = IS_UP || (IS_LEFT && !IS_DOWN);         // 1, 2, 3, 4
        static const bool IS_NEGATIVE = !IS_POSITIVE;                           // 5, 6, 7, 0
};

}       // namespace Geometry
