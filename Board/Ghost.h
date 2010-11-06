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
        static const bool BIT_COLORING = T::SQUARE_COLORING;

        // diagonal directions
        static const size_t SW_NE = (T::WIDTH + N) / 2;         // Southwest-Northeast direction
        static const size_t SE_NW = SW_NE + 1;                  // Southeast-Northwest direction

        // orthogonal directions
        static const size_t WE_EA = SE_NW - SW_NE;              // == 1 by construction
        static const size_t NO_SO = SE_NW + SW_NE;              // == 2 * ((T::WIDTH + G) / 2) + 1

        // number of bits per row pair
        static const size_t BIT_MODULO = NO_SO;
                
        // boundaries of even and odd rows
        static const size_t BIT_LE = BIT_MODULO;                // leftmost bit of even rows
        static const size_t BIT_RE = T::SQUARE_RE;              // rightmost bit of even rows
        static const size_t BIT_LO = SW_NE + BIT_COLORING;      // leftmost bit of odd rows
        static const size_t BIT_RO = BIT_LO + T::ROW_O - 1;     // rightmost bit of odd rows

        // number of used bits
        static const size_t BIT_RANGE = BIT_MODULO * ((T::HEIGHT - 1) / 2) + ((T::HEIGHT % 2)? BIT_RE : BIT_RO) + 1;
};
