#pragma once
#include "Borders.h"
#include "Grid.h"
#include "Angle.h"
#include "../Utilities/IntegerTypes.h"

namespace board {

template
<
        typename SquaresLayout,
        int D = 2,                                              // "demilitarized" rows in the initial position
        int N = 2,                                              // number of ghost bit columns
        int A = Angle::D000                                    // rotation from external to internal grid
>
struct Board: public SquaresLayout
{
        // reflection on template type parameters
        typedef Board<SquaresLayout, D, N, A> T;
        typedef SquaresLayout ExternalGrid;
        typedef typename Rotate<ExternalGrid, A>::type InternalGrid;
        typedef Borders<InternalGrid, N> BordersGrid;

        // reflection on template non-type parameters
        static const int DMZ = D;                               // "demilitarized" rows in the initial position
        static const int ANGLE = A;                             // rotation from external to internal grid                        
        static const int A_PRIME = Inverse<A>::value;           // rotation from internal to external grid

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
        static const int DIRECTION[];                           // the bitwise shifts for all 8 directions

        // square to bit and bit to square conversion tables
        static const int TABLE_SQUARE2BIT[];                    // convert a square to a bit
        static const int TABLE_BIT2SQUARE[];                    // convert a bit to a square
};

}       // namespace board

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Board.hpp"
