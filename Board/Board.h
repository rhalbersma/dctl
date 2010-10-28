#pragma once
#include "Ghosts.h"
#include "Squares.h"
#include "Zones.h"
#include "../Utilities/IntegerTypes.h"

template
<
        typename Layout,
        typename Setup = Zones<>
>
struct Board: public Layout, public Setup
{
        // reflection on template type
        typedef Board<Layout, Setup> T;

        // essential bitboard masks
        static const BitBoard GHOSTS;                           // "ghost" bits
        static const BitBoard INITIAL[];                        // initial position
        static const BitBoard PROMOTION[][2];                   // promotion zones
        static const BitBoard ROW_MASK[][10];
        static const BitBoard COL_MASK[][10];

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
typedef Board< Ghosts< Squares< 4,  4> > > MicroBoard;
typedef Board< Ghosts< Squares< 6,  6> > > MiniBoard;
typedef Board< Ghosts< Squares< 8,  8> > > ChessBoard;
typedef Board< Ghosts< Squares<10, 10> > > InternationalBoard;

// Spanish-Italian board
typedef Board< Ghosts< Squares< 8,  8, true> > > RomanBoard;

// special initial position
typedef Board< Ghosts< Squares< 8,  8> >, Zones<4> > ThaiBoard;

// rectangular boards
typedef Board< Ghosts< Squares<10,  8, true>   >           > SpantsiretiBoard;
typedef Board< Ghosts< Squares<11, 10, true>, 1>, Zones<3> > Ktar11Board;
typedef Board< Ghosts< Squares<12, 10, true>, 1>           > Ktar12Board;

// the default board
typedef InternationalBoard DefaultBoard;

// wrappers to generate bitboard shift masks
template<typename> BitBoard quad_nearest_shifts(BitBoard);
template<typename> BitBoard double_nearest_shifts(BitBoard, size_t);

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "Board.hpp"
