#pragma once
#include "Grid.h"
#include "../Utilities/IntegerTypes.h"

template
<
        typename T,                                             // squares grid layout
        size_t G = 2                                            // number of ghost bit columns
>
struct Ghost: public T
{
        // diagonal directions
        static const size_t SW_NE = (T::WIDTH + G) / 2;         // Southwest-Northeast direction
        static const size_t SE_NW = SW_NE + 1;                  // Southeast-Northwest direction

        // orthogonal directions
        static const size_t WE_EA = SE_NW - SW_NE;              // == 1 by construction
        static const size_t NO_SO = SE_NW + SW_NE;              // == 2 * ((WIDTH + C) / 2) + 1

        // number of bits per row pair
        static const size_t GHOST_MODULO = NO_SO;
                
        // boundaries of even and odd rows
        static const size_t GHOST_LE = GHOST_MODULO;            // leftmost bit of even rows
        static const size_t GHOST_RE = T::SQUARE_RE;            // rightmost bit of even rows
        static const size_t GHOST_LO = SW_NE + T::COLORING;     // leftmost bit of odd rows
        static const size_t GHOST_RO = GHOST_LO + T::ROW_O - 1; // rightmost bit of odd rows

        // number of used bits
        static const size_t BIT_RANGE = GHOST_MODULO * ((T::HEIGHT - 1) / 2) + ((T::HEIGHT % 2)? GHOST_RE : GHOST_RO) + 1;

        // number of available bits
        static const size_t NUM_BITS = 8 * sizeof(BitBoard);    // currently 64-bits
};
