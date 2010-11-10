#pragma once
#include "Transform.h"
#include "../Utilities/IntegerTypes.h"
#include "../Position/Reversible/Side.h"

// the 8 diagonal and orthogonal direction indices for black and white to move
template<bool> struct DirIndex;

template<> 
struct DirIndex<Side::BLACK> 
{ 
        enum { 
                RIGHT = 4, RIGHT_UP = 5, UP = 6, LEFT_UP = 7, LEFT = 0, LEFT_DOWN = 1, DOWN = 2, RIGHT_DOWN = 3 
        }; 
};

template<> 
struct DirIndex<Side::WHITE> 
{ 
        enum { 
                RIGHT = 0, RIGHT_UP = 1, UP = 2, LEFT_UP = 3, LEFT = 4, LEFT_DOWN = 5, DOWN = 6, RIGHT_DOWN = 7 
        }; 
};

template<size_t I>
struct Direction
{
        // diagonality, orthogonality
        static const bool IS_DIAGONAL = I % 2;					// 1, 3, 5, 7
        static const bool IS_ORTHOGONAL = !IS_DIAGONAL;				// 0, 2, 4, 6

        // up, down, right, left
        static const size_t I_L090 = RotateDirection<I, L090>::VALUE;
        static const bool IS_UP = !(I / 4) && (I % 4);				// 1, 2, 3
        static const bool IS_DOWN = (I / 4) && (I % 4);			        // 5, 6, 7
        static const bool IS_RIGHT = Direction<I_L090>::IS_UP;	                // 0, 1, 7
        static const bool IS_LEFT = Direction<I_L090>::IS_DOWN;	                // 3, 4, 5

        // positive, negative
        static const bool IS_POSITIVE = IS_UP || (IS_LEFT && !IS_DOWN);         // 1, 2, 3, 4
        static const bool IS_NEGATIVE = !IS_POSITIVE;				// 5, 6, 7, 0
};
