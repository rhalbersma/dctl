#pragma once
#include "../Utilities/IntegerTypes.h"
#include "../Position/Reversible/Side.h"

template<size_t I>
struct Direction
{
	// directions are subject to arithmetic modulo 8, with the unit element equal to 45 degrees rotation
	enum { DEGREES_045 = 1, DEGREES_090 = 2, DEGREES_135 = 3, DEGREES_180 = 4, DEGREES_360 = 8 };

	// rotated indices (in steps of 45 degrees)
        static const size_t ROTATE_R045 = (I - DEGREES_045) % DEGREES_360;
        static const size_t ROTATE_L045 = (I + DEGREES_045) % DEGREES_360;
        static const size_t ROTATE_R090 = (I - DEGREES_090) % DEGREES_360;
        static const size_t ROTATE_L090 = (I + DEGREES_090) % DEGREES_360;
        static const size_t ROTATE_R135 = (I - DEGREES_135) % DEGREES_360;
        static const size_t ROTATE_L135 = (I + DEGREES_135) % DEGREES_360;
        static const size_t ROTATE_180  = (I + DEGREES_180) % DEGREES_360;

        // the unique alternative direction for men that can only capture in the 2 forward diagonal directions
        static const size_t PROJECT_U90 = (I + DEGREES_090) % DEGREES_180 + ((I / DEGREES_180) * DEGREES_180);

        // positive (negative) directions need bitwise left (right) shifting
        static const bool IS_POSITIVE = I < DEGREES_180;			// 0, 1, 2, 3
        static const bool IS_NEGATIVE = !IS_POSITIVE;				// 4, 5, 6, 7

        // diagonality, orthogonality
        static const bool IS_DIAGONAL = I % 2;					// 1, 3, 5, 7
        static const bool IS_ORTHOGONAL = !IS_DIAGONAL;				// 0, 2, 4, 6

        // up, down, right, left
        static const bool IS_U = !(I / 4) && (I % 4);				// 1, 2, 3
        static const bool IS_D =  (I / 4) && (I % 4);				// 5, 6, 7
        static const bool IS_R = !(ROTATE_R090 / 4) && (ROTATE_R090 % 4);	// 0, 1, 7
        static const bool IS_L =  (ROTATE_R090 / 4) && (ROTATE_R090 % 4);	// 3, 4, 5
};

// the 8 diagonal and orthogonal direction indices for black and white to move
template<bool> struct DirIndex;
template<> struct DirIndex<Side::BLACK> { enum { L = 4, LU = 5, U = 6, RU = 7, R = 0, RD = 1, D = 2, LD = 3 }; };
template<> struct DirIndex<Side::WHITE> { enum { L = 0, LU = 1, U = 2, RU = 3, R = 4, RD = 5, D = 6, LD = 7 }; };
