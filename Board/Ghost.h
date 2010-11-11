#pragma once
#include "Grid.h"
#include "Transform.h"
#include "../Utilities/IntegerTypes.h"

template
<
        typename T,                                     // grid layout of squares
        size_t N                                        // number of ghost bit columns
>
struct Ghost
{
        enum {
                // extract coloring of top-left square
                BIT_PARITY = T::SQUARE_PARITY,

                // diagonal directions
                LEFT_DOWN = (T::WIDTH + N) / 2,         // left-down direction
                RIGHT_DOWN = LEFT_DOWN + 1,             // right-down direction

                // orthogonal directions
                RIGHT = RIGHT_DOWN - LEFT_DOWN,         // right direction == 1 by construction
                DOWN = RIGHT_DOWN + LEFT_DOWN,          // down direction == 2 * ((T::WIDTH + G) / 2) + 1

                // auxiliary directions
                LEFT_UP = RIGHT_DOWN,                   // left-up direction 
                RIGHT_UP = LEFT_DOWN,                   // right-up direction
                LEFT = RIGHT,                           // left direction
                UP = DOWN,                              // up direction

                // number of bits per row pair
                BIT_MODULO = DOWN,
                
                // boundaries of even and odd rows
                BIT_LE = BIT_MODULO,                    // leftmost bit of even rows
                BIT_RE = T::SQUARE_RE,                  // rightmost bit of even rows
                BIT_LO = LEFT_DOWN + BIT_PARITY,        // leftmost bit of odd rows
                BIT_RO = BIT_LO + T::ROW_O - 1,         // rightmost bit of odd rows

                // number of used bits
                BIT_RANGE = BIT_MODULO * ((T::HEIGHT - 1) / 2) + ((T::HEIGHT % 2)? BIT_RE : BIT_RO) + 1
        };
};
