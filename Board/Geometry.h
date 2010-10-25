#pragma once
#include "../Utilities/IntegerTypes.h"

template
<
        size_t H,                                               // height
        size_t W,                                               // width
        bool P = false                                          // parity of top-left square
>
struct Geometry
{
        // reflection on template parameters
        static const size_t HEIGHT = H;
        static const size_t WIDTH = W;
        static const bool PARITY = P;

        // number of squares per row and row pair
        static const size_t ROW_E = (W +  P) / 2;               // number of squares in even rows
        static const size_t ROW_O = (W + !P) / 2;               // number of squares in odd rows
        static const size_t SQUARE_MODULO = W;                  // number of squares per row pair

        // boundaries of even and odd rows
        static const size_t SQUARE_LE = 0;                      // leftmost square of even rows
        static const size_t SQUARE_RE = SQUARE_LE + ROW_E - 1;  // rightmost square of even rows
        static const size_t SQUARE_LO = SQUARE_RE + 1;          // leftmost square of odd rows
        static const size_t SQUARE_RO = SQUARE_LO + ROW_O - 1;  // rightmost square of even rows

        // formula for the number of squares that generalizes to ghost bits
        static const size_t SQUARE_RANGE = SQUARE_MODULO * ((H - 1) / 2) + ((H % 2)? SQUARE_RE : SQUARE_RO) + 1;

        // equivalent formula for the number of squares
        static const size_t NUM_SQUARES = (H * W) / 2 + (H * W * P) % 2;
};
