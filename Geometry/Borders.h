#pragma once
#include <cstddef>

namespace Geometry {

template
<
        typename T,                                             // grid layout of squares
        size_t N                                                // number of ghost bit columns
>
struct Borders
{
        // extract coloring of top-left square
        static const bool PARITY = T::PARITY;

        // diagonal directions
        static const size_t LEFT_DOWN = (T::WIDTH + N) / 2;
        static const size_t RIGHT_DOWN = LEFT_DOWN + 1;

        // orthogonal directions
        static const size_t RIGHT = RIGHT_DOWN - LEFT_DOWN;     // == 1 by construction
        static const size_t DOWN = RIGHT_DOWN + LEFT_DOWN;      // == 2 * ((T::WIDTH + N) / 2) + 1

        // equivalent directions
        static const size_t LEFT_UP = RIGHT_DOWN;
        static const size_t RIGHT_UP = LEFT_DOWN;
        static const size_t LEFT = RIGHT;
        static const size_t UP = DOWN;
               
        // range of row pairs
        static const size_t MODULO = DOWN;
                
        // left (L) and right (R) edges of even (E) and odd (O) rows
        static const size_t EDGE_LE = T::EDGE_LE;
        static const size_t EDGE_RE = T::EDGE_RE;
        static const size_t EDGE_LO = LEFT_DOWN + PARITY;
        static const size_t EDGE_RO = EDGE_LO + T::EDGE_RO - T::EDGE_LO;

        // grid size 
        static const size_t SIZE = MODULO * ((T::HEIGHT - 1) / 2) + ((T::HEIGHT % 2)? EDGE_RE : EDGE_RO) + 1;
};

}       // namespace Geometry
