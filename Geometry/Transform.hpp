#include "Squares.h"

namespace Geometry {

// rotations are subject to arithmetic modulo 360 degrees, with the unit element equal to 45 degrees anti-clockwise
enum { D000 = 0, D045 = 1, D090 = 2, D135 = 3, D180 = 4, D225 = 5, D270 = 6, D315 = 7, D360 = 8 };

template<size_t I, size_t A>
struct AntiClockwise
{
        enum { VALUE = (I + A) % D360 };
};

// an inverse angle has the opposite sign modulo 360 degrees
template<size_t A>
struct Inverse
{
        enum { VALUE = (D360 - A) % D360 };
};

// mirrored upward direction (orthogonal to the original)
template<size_t I>
struct Mirror
{
        enum { U090 = (I + D090) % D180 + (I / D180) * D180 };
};

// rotated direction indices (in steps of 45 degrees anti-clockwise)
template<size_t I>
struct Rotate
{
        enum { 
                I000 = AntiClockwise<I, D000>::VALUE, 
                L045 = AntiClockwise<I, D045>::VALUE, 
                L090 = AntiClockwise<I, D090>::VALUE, 
                L135 = AntiClockwise<I, D135>::VALUE, 
                I180 = AntiClockwise<I, D180>::VALUE, 
                R135 = AntiClockwise<I, D225>::VALUE, 
                R090 = AntiClockwise<I, D270>::VALUE, 
                R045 = AntiClockwise<I, D315>::VALUE, 
                I360 = AntiClockwise<I, D360>::VALUE
        };
};

// identity rotation
template<typename T, int R, int C>
struct RotateCoordinates<T, R, C, D000>
{
        static const int ROW = R;
        static const int COL = C;
};

// rotate 90 degrees right
template<typename T, int R, int C>
struct RotateCoordinates<T, R, C, D270>
{
        static const int ROW = (static_cast<int>(T::WIDTH) - 1) - C;
        static const int COL = R;
};

// rotate 90 degrees left
template<typename T, int R, int C>
struct RotateCoordinates<T, R, C, D090>
{
        static const int ROW = C;
        static const int COL = (static_cast<int>(T::HEIGHT) - 1) - R;
};

// rotate 180 degrees
template<typename T, int R, int C>
struct RotateCoordinates<T, R, C, D180>
{
        static const int ROW = (static_cast<int>(T::HEIGHT) - 1) - R;
        static const int COL = (static_cast<int>(T::WIDTH) - 1) - C;
};

// identity rotation
template<typename In>
struct RotateSquares<In, D000>
{
        typedef In Out;
};

// rotate 90 degrees right
template<typename In>
struct RotateSquares<In, D270>
{
        typedef Squares<In::WIDTH, In::HEIGHT, (In::WIDTH % 2) ^ !In::SQUARE_PARITY> Out;
};

// rotate 90 degree left
template<typename In>
struct RotateSquares<In, D090>
{
        typedef Squares<In::WIDTH, In::HEIGHT, (In::HEIGHT % 2) ^ !In::SQUARE_PARITY> Out;
};

// rotate 180 degrees
template<typename In>
struct RotateSquares<In, D180>
{
        typedef Squares<In::HEIGHT, In::WIDTH, (In::HEIGHT % 2) ^ (In::WIDTH % 2) ^ In::SQUARE_PARITY> Out;
};

}       // namespace Geometry
