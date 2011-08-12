#pragma once

namespace dctl {
namespace board {

template
<
        typename Dimensions,    // dimensions
        int GHOST = 0           // ghost columns
>
class Grid
: 
        public Dimensions
{
public:
        typedef Grid<Dimensions> BaseGrid;

        enum {
                // diagonal directions
                LEFT_DOWN = (Dimensions::WIDTH + GHOST) / 2,
                RIGHT_DOWN = LEFT_DOWN + 1,

                // orthogonal directions
                RIGHT = RIGHT_DOWN - LEFT_DOWN, // == 1 by construction
                DOWN = RIGHT_DOWN + LEFT_DOWN,  // == 2 * ((WIDTH + GHOST) / 2) + 1

                // equivalent directions
                LEFT_UP = RIGHT_DOWN,
                RIGHT_UP = LEFT_DOWN,
                LEFT = RIGHT,
                UP = DOWN,
               
                // range of row pairs
                MODULO = DOWN,
                
                // left (L) and right (R) edges of even (E) and odd (O) rows
                EDGE_LE = BaseGrid::EDGE_LE,
                EDGE_RE = BaseGrid::EDGE_RE,
                EDGE_LO = LEFT_DOWN + Dimensions::PARITY,
                EDGE_RO = EDGE_LO + BaseGrid::EDGE_RO - BaseGrid::EDGE_LO,

                // grid size 
                SIZE = MODULO * ((Dimensions::HEIGHT - 1) / 2) + ((Dimensions::HEIGHT % 2)? EDGE_RE : EDGE_RO) + 1
        };
};

// partial specialization for grids without ghost columns
template<typename Dimensions>
class Grid<Dimensions, 0>
: 
        public Dimensions
{
private:
        enum {
                // range of even (E) and odd (O) rows
                ROW_E = (Dimensions::WIDTH +  Dimensions::PARITY) / 2,
                ROW_O = (Dimensions::WIDTH + !Dimensions::PARITY) / 2
        };

public:
        enum {
                // range of row pairs
                MODULO = Dimensions::WIDTH,

                // left (L) and right (R) edges of even (E) and odd (O) rows
                EDGE_LE = 0,
                EDGE_RE = EDGE_LE + ROW_E - 1,
                EDGE_LO = EDGE_RE + 1,
                EDGE_RO = EDGE_LO + ROW_O - 1,

                // == (H * W) / 2 + (H * W * P) % 2
                SIZE = MODULO * ((Dimensions::HEIGHT - 1) / 2) + ((Dimensions::HEIGHT % 2)? EDGE_RE : EDGE_RO) + 1
        };
};

}       // namespace board
}       // namespace dctl
