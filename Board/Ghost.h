#pragma once
#include "Grid.h"
#include "Transform.h"
#include "../Utilities/IntegerTypes.h"

template
<
        typename T,                                             // grid layout of squares
        size_t N                                                // number of ghost bit columns
>
struct Ghost
{
        // extract coloring of top-left square
        static const bool BIT_PARITY = T::SQUARE_PARITY;

        // diagonal directions
        static const size_t LEFT_DOWN = (T::WIDTH + N) / 2;     // left-down direction
        static const size_t RIGHT_DOWN = LEFT_DOWN + 1;         // right-down direction

        // orthogonal directions
        static const size_t RIGHT = RIGHT_DOWN - LEFT_DOWN;     // right direction == 1 by construction
        static const size_t DOWN = RIGHT_DOWN + LEFT_DOWN;      // down direction == 2 * ((T::WIDTH + G) / 2) + 1

        // equivalent directions
        static const size_t LEFT_UP = RIGHT_DOWN;               // left-up direction 
        static const size_t RIGHT_UP = LEFT_DOWN;               // right-up direction
        static const size_t LEFT = RIGHT;                       // left direction
        static const size_t UP = DOWN;                          // up direction
               
        // number of bits per row pair
        static const size_t BIT_MODULO = DOWN;              
                
        // boundaries of even and odd rows
        static const size_t BIT_LE = BIT_MODULO;                // leftmost bit of even rows
        static const size_t BIT_RE = T::SQUARE_RE;              // rightmost bit of even rows
        static const size_t BIT_LO = LEFT_DOWN + BIT_PARITY;    // leftmost bit of odd rows
        static const size_t BIT_RO = BIT_LO + T::ROW_O - 1;     // rightmost bit of odd rows

        // number of used bits
        static const size_t BIT_RANGE = BIT_MODULO * ((T::HEIGHT - 1) / 2) + ((T::HEIGHT % 2)? BIT_RE : BIT_RO) + 1;
};
