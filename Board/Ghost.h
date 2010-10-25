#pragma once
#include "Geometry.h"
#include "../Utilities/IntegerTypes.h"

template
<
        typename Geometry,                                      // geometric square layout
        size_t G                                                // number of ghost columns
>
struct Ghost: public Geometry
{
        // extraction of inherited template parameters
        static const size_t H = Geometry::HEIGHT;
        static const size_t W = Geometry::WIDTH;
        static const bool P = Geometry::PARITY;

        // diagonal directions
        static const size_t SW_NE = (W + G) / 2;                // Southwest-Northeast direction
        static const size_t SE_NW = SW_NE + 1;                  // Southeast-Northwest direction

        // orthogonal directions
        static const size_t WE_EA = SE_NW - SW_NE;              // == 1 by construction
        static const size_t NO_SO = SE_NW + SW_NE;              // == 2 * ((W + G) / 2) + 1

        // number of bits per row pair
        static const size_t GHOST_MODULO = NO_SO;
                
        // boundaries of even and odd rows
        static const size_t GHOST_LE = GHOST_MODULO;            // left of even rows
        static const size_t GHOST_RE = SQUARE_RE;               // right of even rows
        static const size_t GHOST_LO = SW_NE + P;               // left of odd rows
        static const size_t GHOST_RO = GHOST_LO + ROW_O - 1;    // right of odd rows

        // number of used bits
        static const size_t BIT_RANGE = GHOST_MODULO * ((H - 1) / 2) + ((H % 2)? GHOST_RE : GHOST_RO) + 1;

        // number of available bits
        static const size_t NUM_BITS = 8 * sizeof(BitBoard);    // currently 64-bits
};
