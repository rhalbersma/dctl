#pragma once
#include <cstddef>

namespace Geometry {

template
<
        typename T,                                                     // grid layout of squares
        size_t N                                                        // number of ghost bit columns
>
struct Borders
{
        // extract coloring of top-left square
        static const bool PARITY = T::PARITY;

        // diagonal directions
        static const size_t LEFT_DOWN = (T::WIDTH + N) / 2;             // left-down direction
        static const size_t RIGHT_DOWN = LEFT_DOWN + 1;                 // right-down direction

        // orthogonal directions
        static const size_t RIGHT = RIGHT_DOWN - LEFT_DOWN;             // right direction == 1 by construction
        static const size_t DOWN = RIGHT_DOWN + LEFT_DOWN;              // down direction == 2 * ((T::WIDTH + N) / 2) + 1

        // equivalent directions
        static const size_t LEFT_UP = RIGHT_DOWN;                       // left-up direction 
        static const size_t RIGHT_UP = LEFT_DOWN;                       // right-up direction
        static const size_t LEFT = RIGHT;                               // left direction
        static const size_t UP = DOWN;                                  // up direction
               
        // number of bits per row pair
        static const size_t MODULO = DOWN;
                
        // boundaries of even and odd rows
        static const size_t EDGE_LE = MODULO;                           // left edge of even rows
        static const size_t EDGE_RE = T::EDGE_RE;                       // right edge of even rows
        static const size_t EDGE_LO = LEFT_DOWN + PARITY;               // left edge of odd rows
        static const size_t EDGE_RO = EDGE_LO + T::ROW_O - 1;     	// right edge of odd rows

        // number of used bits
        static const size_t SIZE = MODULO * ((T::HEIGHT - 1) / 2) + ((T::HEIGHT % 2)? EDGE_RE : EDGE_RO) + 1;
};

}       // namespace Geometry
