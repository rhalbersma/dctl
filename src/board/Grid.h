#pragma once

namespace board {

template
<
        typename Dimensions,    // dimensions
        int G = 0               // ghost columns
>
class Grid: public Dimensions
{
public:
        typedef Grid<Dimensions> BaseGrid;

        // diagonal directions
        static const int LEFT_DOWN = (WIDTH + G) / 2;
        static const int RIGHT_DOWN = LEFT_DOWN + 1;

        // orthogonal directions
        static const int RIGHT = RIGHT_DOWN - LEFT_DOWN;        // == 1 by construction
        static const int DOWN = RIGHT_DOWN + LEFT_DOWN;         // == 2 * ((T::WIDTH + N) / 2) + 1

        // equivalent directions
        static const int LEFT_UP = RIGHT_DOWN;
        static const int RIGHT_UP = LEFT_DOWN;
        static const int LEFT = RIGHT;
        static const int UP = DOWN;
               
        // range of row pairs
        static const int MODULO = DOWN;
                
        // left (L) and right (R) edges of even (E) and odd (O) rows
        static const int EDGE_LE = BaseGrid::EDGE_LE;
        static const int EDGE_RE = BaseGrid::EDGE_RE;
        static const int EDGE_LO = LEFT_DOWN + PARITY;
        static const int EDGE_RO = EDGE_LO + BaseGrid::EDGE_RO - BaseGrid::EDGE_LO;

        // grid size 
        static const int SIZE = MODULO * ((HEIGHT - 1) / 2) + ((HEIGHT % 2)? EDGE_RE : EDGE_RO) + 1;
};

// partial specialization for grids without ghost columns
template<typename Dimensions>
class Grid<Dimensions, 0>: public Dimensions
{
private:
        // range of even (E) and odd (O) rows
        static const int ROW_E = (WIDTH +  PARITY) / 2;
        static const int ROW_O = (WIDTH + !PARITY) / 2;

public:
        // range of row pairs
        static const int MODULO = WIDTH;

        // left (L) and right (R) edges of even (E) and odd (O) rows
        static const int EDGE_LE = 0;
        static const int EDGE_RE = EDGE_LE + ROW_E - 1;
        static const int EDGE_LO = EDGE_RE + 1;
        static const int EDGE_RO = EDGE_LO + ROW_O - 1;

        // == (H * W) / 2 + (H * W * P) % 2
        static const int SIZE = MODULO * ((HEIGHT - 1) / 2) + ((HEIGHT % 2)? EDGE_RE : EDGE_RO) + 1;
};

}       // namespace board
