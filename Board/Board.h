#pragma once
#include "Grid.h"
#include "Ghost.h"
#include "Transform.h"
#include "../Utilities/IntegerTypes.h"

template
<
        typename GridLayout,
        size_t D = 2,
        size_t N = 1,
        size_t A = D000
>
struct Board: public GridLayout
{
        // reflection on template type parameters
        typedef Board<GridLayout, D, N, A> T;
        typedef GridLayout ExternalGrid;
        typedef typename RotateGrid<ExternalGrid, A>::Out InternalGrid;
        typedef Ghost<InternalGrid, N> GhostStructure;

        // reflection on template non-type parameters
        static const size_t DMZ = D;                            // "demilitarized" rows in the initial position
        static const size_t ANGLE = A;                          // rotation from external to internal grid                        
        static const size_t A_PRIME = InverseAngle<A>::VALUE;   // rotation from internal to external grid

        // essential bitboard masks
        static const BitBoard SQUARES;                          // bit mask of legal squares, excluding ghost squares
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
        static const size_t DIR[];                              // the bitwise shifts for all 8 directions

        // square to bit and bit to square conversion tables
        static const int TABLE_SQUARE2BIT[];                    // convert a square to a bit
        static const int TABLE_BIT2SQUARE[];                    // convert a bit to a square
};

// square boards
typedef Board< Grid< 4,  4> > MicroBoard;
typedef Board< Grid< 6,  6> > MiniBoard;
typedef Board< Grid< 8,  8> > ChessBoard;
typedef Board< Grid<10, 10> > InternationalBoard;

// Spanish-Italian board
typedef Board< Grid< 8,  8, true> > RomanBoard;

// special initial position
typedef Board< Grid< 8,  8>, 4 > ThaiBoard;

// rectangular boards
typedef Board< Grid<10,  8, true>             > SpantsiretiBoard;
typedef Board< Grid<11, 10, true>, 3, 1       > Ktar11Board;
typedef Board< Grid<12, 10, true>, 2, 1, R090 > Ktar12Board;

// the default board
typedef InternationalBoard DefaultBoard;

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Board.hpp"
