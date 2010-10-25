#pragma once
#include "../Utilities/IntegerTypes.h"
#include "../Position/Reversible/Side.h"

// directions are subject to arithmetic modulo 8, with the unit element equal to a 45 degrees rotation
enum { D000 = 0, L045 = 1, L090 = 2, L135 = 3, D180 = 4, R045 = 5, R090 = 6, R135 = 7, D360 = 8 };

// the 8 diagonal and orthogonal direction indices for black and white to move
template<bool> struct DirIndex;
template<> struct DirIndex<Side::BLACK> { enum { L = 4, LU = 5, U = 6, RU = 7, R = 0, RD = 1, D = 2, LD = 3 }; };
template<> struct DirIndex<Side::WHITE> { enum { L = 0, LU = 1, U = 2, RU = 3, R = 4, RD = 5, D = 6, LD = 7 }; };

// rotated indices (in steps of 45 degrees)
template<size_t I, size_t ANGLE>
struct Rotate
{
        static const size_t VALUE = (I + ANGLE) % D360;
};

// the unique alternative direction for men that can only capture in the 2 forward diagonal directions
template<size_t I>
struct Project
{
        static const size_t U090 = (I + L090) % D180 + (I / D180) * D180;
};

template<size_t I>
struct Direction
{
        // positive (negative) directions need bitwise left (right) shifting
        static const bool IS_POSITIVE = I < D180;			        // 0, 1, 2, 3
        static const bool IS_NEGATIVE = !IS_POSITIVE;				// 4, 5, 6, 7

        // diagonality, orthogonality
        static const bool IS_DIAGONAL = I % 2;					// 1, 3, 5, 7
        static const bool IS_ORTHOGONAL = !IS_DIAGONAL;				// 0, 2, 4, 6

        // up, down, right, left
        static const size_t ROTATE_R090 = Rotate<I, R090>::VALUE;
        static const bool IS_U = !(I / 4) && (I % 4);				// 1, 2, 3
        static const bool IS_D =  (I / 4) && (I % 4);				// 5, 6, 7
        static const bool IS_R = !(ROTATE_R090 / 4) && (ROTATE_R090 % 4);	// 0, 1, 7
        static const bool IS_L =  (ROTATE_R090 / 4) && (ROTATE_R090 % 4);	// 3, 4, 5
};
