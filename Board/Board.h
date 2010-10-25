#pragma once
#include "Geometry.h"
#include "Ghost.h"
#include "../Utilities/IntegerTypes.h"

template
<
        typename Geometry,
        size_t G = 2,
        size_t N = 2
>
struct Board: public Ghost<Geometry, G>
{
        // reflection on template type
        typedef Board<Geometry, G, N> T;

        // reflection on template parameters
        static const size_t NEUTRAL_ZONE = N;

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
typedef Board< Geometry< 4,  4> > MicroBoard;
typedef Board< Geometry< 6,  6> > MiniBoard;
typedef Board< Geometry< 8,  8> > ChessBoard;
typedef Board< Geometry<10, 10> > InternationalBoard;

// Spanish-Italian board
typedef Board< Geometry<8,  8, true> > RomanBoard;

// special initial position
typedef Board< Geometry<8,  8>, 2, 4> ThaiBoard;

// rectangular boards
typedef Board< Geometry<10,  8, true> > SpantsiretiBoard;
typedef Board< Geometry<11, 10, true>, 1, 2> Ktar11Board;
typedef Board< Geometry<12, 10, true>, 1, 3> Ktar12Board;

// the default board
typedef InternationalBoard DefaultBoard;

// wrappers to generate bitboard shift masks
template<typename> BitBoard quad_nearest_shifts(BitBoard);
template<typename> BitBoard double_nearest_shifts(BitBoard, size_t);

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "Board.hpp"
