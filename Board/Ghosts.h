#pragma once
#include "Squares.h"
#include "../Utilities/IntegerTypes.h"

template
<
        typename Geometry,                                      // squares geometric layout
        size_t G = 2                                            // number of ghost bit columns
>
struct Ghosts: public Geometry
{
        // diagonal directions
        static const size_t SW_NE = (WIDTH + G) / 2;            // Southwest-Northeast direction
        static const size_t SE_NW = SW_NE + 1;                  // Southeast-Northwest direction

        // orthogonal directions
        static const size_t WE_EA = SE_NW - SW_NE;              // == 1 by construction
        static const size_t NO_SO = SE_NW + SW_NE;              // == 2 * ((WIDTH + C) / 2) + 1

        // number of bits per row pair
        static const size_t GHOST_MODULO = NO_SO;
                
        // boundaries of even and odd rows
        static const size_t GHOST_LE = GHOST_MODULO;            // leftmost bit of even rows
        static const size_t GHOST_RE = SQUARE_RE;               // rightmost bit of even rows
        static const size_t GHOST_LO = SW_NE + COLORING;        // leftmost bit of odd rows
        static const size_t GHOST_RO = GHOST_LO + ROW_O - 1;    // rightmost bit of odd rows

        // number of used bits
        static const size_t BIT_RANGE = GHOST_MODULO * ((HEIGHT - 1) / 2) + ((HEIGHT % 2)? GHOST_RE : GHOST_RO) + 1;

        // number of available bits
        static const size_t NUM_BITS = 8 * sizeof(BitBoard);    // currently 64-bits
};
