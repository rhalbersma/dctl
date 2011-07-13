#pragma once
#include "Grid.h"
#include "Structure.h"
#include "../Utilities/IntegerTypes.h"

namespace board {

template
<
        typename Dimensions,                                    // height, width and parity
        typename Structure = Structure<>                        // DMZ, ghosts and internal rotation
>
class Board: public Dimensions, public Structure
{
public:
        // reflection on template parameters
        typedef Board<Dimensions, Structure> B;

        // external and internal grids
        typedef Grid<Dimensions> ExternalGrid;
        typedef Grid<typename Rotate<Dimensions, Structure::ANGLE>::type, Structure::GHOSTS> InternalGrid;

        // essential bitboard masks
        static const BitBoard SQUARES;                          // bit mask of legal squares, excluding borders
        static const BitBoard INITIAL[];                        // initial position
        static const BitBoard PROMOTION[][2];                   // promotion zones
        static const BitBoard ROW_MASK[][12];                   // bit masks for the rows
        static const BitBoard COL_MASK[][12];                   // bit masks for the columns

        // auxiliary bitboard masks
        static const BitBoard QUAD_NEAREST_NEIGHBOR_MAGIC;      // shifting bits in 4 directions
        static const BitBoard DOUBLE_NEAREST_NEIGHBOR_MAGIC[];  // shifting bits in 2 directions
        static const BitBoard MAN_JUMP_GROUP[];                 // families of squares reachable by jumping men
        static const BitBoard JUMPABLE[];                       // squares from which a jump is possible in a direction

        // arrays of directions
        static const int SHIFT[];                               // the bitwise shifts for all 8 directions

        static bool is_valid(int);
        static int begin();
        static int end();
        static int square2bit(int);
        static int bit2square(int);

private:
        // square to bit and bit to square conversion tables
        static const int SQUARE2BIT[];                          // convert a square to a bit
        static const int BIT2SQUARE[];                          // convert a bit to a square
};

}       // namespace board

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Board.hpp"
