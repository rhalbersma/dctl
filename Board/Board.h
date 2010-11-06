#pragma once
#include "Grid.h"
#include "Ghost.h"
#include "Transform.h"
#include "../Utilities/IntegerTypes.h"

template
<
        typename GridLayout,
        size_t D = 2,
        size_t N = 2,
        size_t A = D000,
        typename GhostStructure = Ghost<typename RotateGrid<GridLayout, A>::Out, N>
>
struct Board: public GridLayout
{
        // reflection on template type
        typedef Board<GridLayout, D, N, A, GhostStructure> T;

        // reflection on template parameters
        static const size_t DMZ = D;
        static const size_t ANGLE_S2B = A;
        static const size_t ANGLE_B2S = InverseRotation<A>::VALUE;
        typedef                     GridLayout          ExternalGrid;
        typedef typename RotateGrid<GridLayout, A>::Out InternalGrid;
        typedef GhostStructure G;

        // essential bitboard masks
        static const BitBoard GHOSTS;                           // "ghost" bits
        static const BitBoard SQUARES;
        static const BitBoard INITIAL[];                        // initial position
        static const BitBoard PROMOTION[][2];                   // promotion zones
        static const BitBoard ROW_MASK[][12];
        static const BitBoard COL_MASK[][12];

        // auxiliary bitboard masks
        static const BitBoard QUAD_NEAREST_NEIGHBOR_MAGIC;      // shifting bits in 4 directions
        static const BitBoard DOUBLE_NEAREST_NEIGHBOR_MAGIC[];  // shifting bits in 2 directions
        static const BitBoard MAN_JUMP_GROUP[];                 // families of squares reachable by jumping men
        static const BitBoard JUMPABLE[];                       // squares from which a jump is possible in a direction

        // arrays of directions
        static const size_t DIR[];                              // the bitwise shifts corresponding to all 8 directions

        // square to bit and bit to square conversion tables
        static const size_t TABLE_SQUARE2BIT[];                 // convert a square to a bit
        static const size_t TABLE_BIT2SQUARE[];                 // convert a bit to a square
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
typedef Board< Grid<12, 10, true>, 2, 1, L090 > Ktar12Board;

// the default board
typedef InternationalBoard DefaultBoard;

// wrappers to generate bitboard shift masks
template<typename> BitBoard quad_nearest_shifts(BitBoard);
template<typename> BitBoard double_nearest_shifts(BitBoard, size_t);

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "Board.hpp"
