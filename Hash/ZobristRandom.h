#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename Index>
struct ZobristRandom
{
        static Index xor_rand(BitBoard, const Index*);  // xor with random numbers matching a bitboard's set 1-bits
        static Index xor_rand(PlyCount, const Index*);  // xor with random numbers conditional on a ply count
        static Index xor_rand(bool, Index);             // xor with a random number conditional on a boolean

        static const Index PIECES[2][64];
        static const Index KINGS[64];
        static const Index SIDE;
        static const Index SAME_KING[2][64];
        static const Index SAME_MOVES[2][8];
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "ZobristRandom.hpp"
