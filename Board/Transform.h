#pragma once
#include "../Utilities/IntegerTypes.h"

// rotations are subject to arithmetic modulo 8, with the unit element equal to a 45 degrees rotation
enum { D000 = 0, L045 = 1, L090 = 2, L135 = 3, D180 = 4, R045 = 5, R090 = 6, R135 = 7, D360 = 8 };

template<size_t ANGLE>
struct InverseRotation
{
        static const size_t VALUE = (D360 - ANGLE) % D360;
};

// mirrored upward direction (orthogonal to the original)
template<size_t I>
struct MirrorDirection
{
        static const size_t U090 = (I + L090) % D180 + (I / D180) * D180;
};

// rotated direction indices (in steps of 45 degrees)
template<size_t I, size_t ANGLE>
struct RotateDirection
{
        static const size_t VALUE = (I + ANGLE) % D360;
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
        typedef Grid<In::WIDTH, In::HEIGHT, (In::HEIGHT % 2) ^ !In::SQUARE_COLORING> Out;
};

// rotate 90 degree left
template<typename In>
struct RotateGrid<In, L090>
{
        typedef Grid<In::WIDTH, In::HEIGHT, (In::WIDTH % 2) ^ !In::SQUARE_COLORING> Out;
};

// rotate 180 degrees
template<typename In>
struct RotateGrid<In, D180>
{
        typedef Grid<In::HEIGHT, In::WIDTH, (In::HEIGHT % 2) ^ (In::WIDTH % 2) ^ In::SQUARE_COLORING> Out;
};

// identity rotation
template<typename T, size_t R, size_t C, size_t = D000>
struct RotateCoordinate
{
        static const size_t ROW = R;
        static const size_t COL = C;
};

// rotate 90 degrees right
template<typename T, size_t R, size_t C>
struct RotateCoordinate<T, R, C, R090>
{
        static const size_t ROW = C;
        static const size_t COL = (T::HEIGHT - 1) - R;
};

// rotate 90 degree left
template<typename T, size_t R, size_t C>
struct RotateCoordinate<T, R, C, L090>
{
        static const size_t ROW = (T::WIDTH - 1) - C;
        static const size_t COL = R;
};

// rotate 180 degrees
template<typename T, size_t R, size_t C>
struct RotateCoordinate<T, R, C, D180>
{
        static const size_t ROW = (T::HEIGHT - 1) - R;
        static const size_t COL = (T::WIDTH - 1) - C;
};
