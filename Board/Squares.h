#pragma once
#include "../Position/Reversible/Side.h"
#include "../Utilities/IntegerTypes.h"

template
<
        size_t H,                                               // height
        size_t W,                                               // width
        bool C = Side::BLACK                                    // coloring of top-left square
>
struct Squares
{
        // reflection on template parameters
        static const size_t HEIGHT = H;
        static const size_t WIDTH = W;
        static const bool COLORING = C;

        // number of squares per row and row pair
        static const size_t ROW_E = (W +  C) / 2;               // number of squares in even rows
        static const size_t ROW_O = (W + !C) / 2;               // number of squares in odd rows
        static const size_t SQUARE_MODULO = W;                  // number of squares per row pair

        // boundaries of even and odd rows
        static const size_t SQUARE_LE = 0;                      // leftmost square of even rows
        static const size_t SQUARE_RE = SQUARE_LE + ROW_E - 1;  // rightmost square of even rows
        static const size_t SQUARE_LO = SQUARE_RE + 1;          // leftmost square of odd rows
        static const size_t SQUARE_RO = SQUARE_LO + ROW_O - 1;  // rightmost square of even rows

        // formula for the number of squares that generalizes to ghost bits
        static const size_t SQUARE_RANGE = SQUARE_MODULO * ((H - 1) / 2) + ((H % 2)? SQUARE_RE : SQUARE_RO) + 1;

        // equivalent formula for the number of squares
        static const size_t NUM_SQUARES = (H * W) / 2 + (H * W * C) % 2;
};

#include "Direction.h"

// identity rotation
template<typename In, size_t = D000>
struct RotateSquares
{
        typedef In Out;
};

// rotate 90 degrees right
template<typename In>
struct RotateSquares<In, R090>
{
        typedef Squares<In::WIDTH, In::HEIGHT, (In::HEIGHT % 2) ^ !In::COLORING> Out;
};

// rotate 90 degree left
template<typename In>
struct RotateSquares<In, L090>
{
        typedef Squares<In::WIDTH, In::HEIGHT, (In::WIDTH % 2) ^ !In::COLORING> Out;
};

// rotate 180 degrees
template<typename In>
struct RotateSquares<In, D180>
{
        typedef Squares<In::HEIGHT, In::WIDTH, (In::HEIGHT % 2) ^ (In::WIDTH % 2) ^ In::COLORING> Out;
};
