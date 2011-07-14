#pragma once
#include "Angle.h"
#include "Transform.h"
#include "../Utilities/TemplateTricks.h"

namespace board {

template<int I>
class Traits
{
private:
        static const int I_L090 = Rotate<Int2Type<I>, Angle::L090>::value;

public:
        // diagonality, orthogonality
        static const bool IS_DIAGONAL = I % 2;                          // 1, 3, 5, 7
        static const bool IS_ORTHGONAL = !IS_DIAGONAL;                  // 0, 2, 4, 6

        // up, down, right, left
        static const bool IS_UP = !(I / 4) && (I % 4);                  // 1, 2, 3
        static const bool IS_DOWN = (I / 4) && (I % 4);                 // 5, 6, 7
        static const bool IS_RIGHT = Traits<I_L090>::IS_UP;             // 0, 1, 7
        static const bool IS_LEFT = Traits<I_L090>::IS_DOWN;            // 3, 4, 5

        // positive, negative
        static const bool IS_POSITIVE = IS_UP || (IS_LEFT && !IS_DOWN); // 1, 2, 3, 4
        static const bool IS_NEGATIVE = !IS_POSITIVE;                   // 5, 6, 7, 0
};

}       // namespace board
