#pragma once
#include "../Utilities/IntegerTypes.h"

template
<
        size_t H,       // board height
        size_t W,       // board width
        bool P = false, // parity of top-left square
        size_t N = 2,   // neutral rows between pieces in the initial position
        size_t G = 2    // number of ghost columns
>
struct Board
{
        // reflection on template type
        typedef Board<H, W, P, N, G> T;

        // reflection on template parameters
        static const size_t HEIGHT = H;
        static const size_t WIDTH = W;
        static const bool PARITY = P;
        static const size_t NEUTRAL_ZONE = N;
        static const size_t GHOST_COLUMNS = G;

        // number of squares 
        static const size_t NUM_SQUARES = (H * W) / 2 + (H * W * P) % 2;

        // square layout
        static const size_t SQUARE_MODULO = W;                  // number of squares per row pair
        static const size_t SQUARE_LE = 0;                      // leftof even rows
        static const size_t SQUARE_RE = W / 2 + P - 1;          // right of even rows
        static const size_t SQUARE_LO = W / 2 + P;              // left of odd rows
        static const size_t SQUARE_RO = W - 1;                  // right of even rows

        // diagonal directions
        static const size_t SW_NE = (W + G) / 2;
        static const size_t SE_NW = SW_NE + 1;

        // orthogonal directions
        static const size_t WE_EA = SE_NW - SW_NE;              // == 1 by construction
        static const size_t NO_SO = SE_NW + SW_NE;

        // (minimum) number of bits
        static const size_t MIN_BITS = NUM_SQUARES + (H - 1) + !(W % 2) * ((H - !P) / 2);
        static const size_t NUM_BITS = 8 * sizeof(BitBoard);    // currently 64-bits

        // ghost bit layout
        static const size_t GHOST_MODULO = NO_SO;               // number of bits per row pair
        static const size_t GHOST_LE = (W + 1) + !(W % 2);      // left of even rows
        static const size_t GHOST_RE = (W / 2) + P * (W % 2);   // right of even rows
        static const size_t GHOST_LO = (W / 2) + P;             // left of odd rows
        static const size_t GHOST_RO = (W + 1) + !(W % 2) * P;  // right of odd rows

        // essential bitboard masks
        static const BitBoard GHOSTS;                           // "ghost" bits
        static const BitBoard INITIAL[];                        // initial position
        static const BitBoard TO_PROMOTION[][2];                // promotion zones
        static const BitBoard ROW_MASK[][10];
        static const BitBoard COL_MASK[][10];

        // auxiliary bitboard masks
        static const BitBoard QUAD_NEAREST_NEIGHBOR_MAGIC;      // shifting bits in 4 directions
        static const BitBoard DOUBLE_NEAREST_NEIGHBOR_MAGIC[];  // shifting bits in 2 directions
        static const BitBoard MAN_JUMP_GROUP[];                 // families of squares reachable by jumping men
        static const BitBoard JUMPABLE[];                       // squares from which a jump is possible in a direction

        // arrays of directions
        static const size_t DIR[];                              // the bitwise shifts corresponding to all 8 directions

        // square to bit and bit to square NonConversion tables
        static const size_t TABLE_SQUARE2BIT[];                 // convert a square to a bit
        static const size_t TABLE_BIT2SQUARE[];                 // convert a bit to a square
};

// square boards
typedef Board< 4,  4> MicroBoard;
typedef Board< 6,  6> MiniBoard;
typedef Board< 8,  8> ChessBoard;
typedef Board<10, 10> InternationalBoard;

// Spanish-Italian board
typedef Board< 8,  8, true> RomanBoard;

// special initial position
typedef Board< 8,  8, false, 4> ThaiBoard;

// rectangular boards
typedef Board<10,  8, true> SpantsiretiBoard;

// the default board
typedef InternationalBoard DefaultBoard;

// wrappers to generate bitboard shift masks
template<typename> BitBoard quad_nearest_shifts(BitBoard);
template<typename> BitBoard double_nearest_shifts(BitBoard, size_t);

// squares lie within the range 1...NUM_SQUARES
template<typename> bool is_valid_square(size_t);

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "Board.hpp"
