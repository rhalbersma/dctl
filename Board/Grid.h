#pragma once
#include "../Utilities/IntegerTypes.h"

template
<
        size_t H,                                       // height
        size_t W,                                       // width
        bool P = false                                  // coloring of top-left square
>
struct Grid
{
        enum {
                // store template parameters
                HEIGHT = H,
                WIDTH = W,
                SQUARE_PARITY = P,

                // number of squares per row and row pair
                ROW_E = (W +  P) / 2,                   // number of squares in even rows
                ROW_O = (W + !P) / 2,                   // number of squares in odd rows
                SQUARE_MODULO = W,                      // number of squares per row pair

                // boundaries of even and odd rows
                SQUARE_LE = 0,                          // leftmost square of even rows
                SQUARE_RE = SQUARE_LE + ROW_E - 1,      // rightmost square of even rows
                SQUARE_LO = SQUARE_RE + 1,              // leftmost square of odd rows
                SQUARE_RO = SQUARE_LO + ROW_O - 1,      // rightmost square of even rows

                // formula for the number of squares that generalizes to ghost bits
                SQUARE_RANGE = SQUARE_MODULO * ((H - 1) / 2) + ((H % 2)? SQUARE_RE : SQUARE_RO) + 1,

                // equivalent formula for the number of squares
                NUM_SQUARES = (H * W) / 2 + (H * W * P) % 2
        };
};
