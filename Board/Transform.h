#pragma once
#include "../Utilities/IntegerTypes.h"

// rotations are subject to arithmetic modulo 8, with the unit element equal to a 45 degrees
enum { D000 = 0, L045 = 1, L090 = 2, L135 = 3, D180 = 4, R135 = 5, R090 = 6, R045 = 7, D360 = 8 };

template<size_t A>
struct InverseAngle
{
        static const size_t VALUE = (D360 - A) % D360;
};

// mirrored upward direction (orthogonal to the original)
template<size_t I>
struct MirrorDirection
{
        static const size_t U090 = (I + L090) % D180 + (I / D180) * D180;
};

// rotated direction indices (in steps of 45 degrees)
template<size_t I, size_t A>
struct RotateDirection
{
        static const size_t VALUE = (I + A) % D360;
};

// identity rotation
template<typename In, size_t = D000>
struct RotateGrid
{
        typedef In Out;
};

// rotate 90 degrees right
template<typename In>
struct RotateGrid<In, R090>
{
        typedef Grid<In::WIDTH, In::HEIGHT, (In::HEIGHT % 2) ^ !In::SQUARE_PARITY> Out;
};

// rotate 90 degree left
template<typename In>
struct RotateGrid<In, L090>
{
        typedef Grid<In::WIDTH, In::HEIGHT, (In::WIDTH % 2) ^ !In::SQUARE_PARITY> Out;
};

// rotate 180 degrees
template<typename In>
struct RotateGrid<In, D180>
{
        typedef Grid<In::HEIGHT, In::WIDTH, (In::HEIGHT % 2) ^ (In::WIDTH % 2) ^ In::SQUARE_PARITY> Out;
};

// identity rotation
template<typename T, int R, int C, size_t = D000>
struct RotateCoordinate
{
        enum {
                ROW = R,
                COL = C
        };
};

// rotate 90 degrees right
template<typename T, int R, int C>
struct RotateCoordinate<T, R, C, R090>
{
        enum {
                ROW = C,
                COL = (T::HEIGHT - 1) - R
        };
};

// rotate 90 degree left
template<typename T, int R, int C>
struct RotateCoordinate<T, R, C, L090>
{
        enum {
                ROW = (T::WIDTH - 1) - C,
                COL = R
        };
};

// rotate 180 degrees
template<typename T, int R, int C>
struct RotateCoordinate<T, R, C, D180>
{
        enum {
                ROW = (T::HEIGHT - 1) - R,
                COL = (T::WIDTH - 1) - C
        };
};
