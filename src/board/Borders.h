#pragma once

namespace board {

template
<
        typename T,                                             // grid layout of squares
        int N                                                   // number of ghost bit columns
>
struct Borders
{
        // diagonal directions
        static const int LEFT_DOWN = (T::WIDTH + N) / 2;
        static const int RIGHT_DOWN = LEFT_DOWN + 1;

        // orthogonal directions
        static const int RIGHT = RIGHT_DOWN - LEFT_DOWN;        // == 1 by construction
        static const int DOWN = RIGHT_DOWN + LEFT_DOWN;         // == 2 * ((T::WIDTH + N) / 2) + 1

        // equivalent directions
        static const int LEFT_UP = RIGHT_DOWN;
        static const int RIGHT_UP = LEFT_DOWN;
        static const int LEFT = RIGHT;
        static const int UP = DOWN;
               
        // extract coloring of top-left square
        static const bool PARITY = T::PARITY;

        // range of row pairs
        static const int MODULO = DOWN;
                
        // left (L) and right (R) edges of even (E) and odd (O) rows
        static const int EDGE_LE = T::EDGE_LE;
        static const int EDGE_RE = T::EDGE_RE;
        static const int EDGE_LO = LEFT_DOWN + PARITY;
        static const int EDGE_RO = EDGE_LO + T::EDGE_RO - T::EDGE_LO;

        // grid size 
        static const int SIZE = MODULO * ((T::HEIGHT - 1) / 2) + ((T::HEIGHT % 2)? EDGE_RE : EDGE_RO) + 1;
};

}       // namespace board
