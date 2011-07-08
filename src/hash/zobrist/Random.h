#pragma once
#include "../../Utilities/IntegerTypes.h"

namespace hash {
namespace zobrist {

template<typename Index>
struct Random
{
        static Index xor_rand(BitBoard, const Index*);  // xor with random numbers matching a bitboard's set 1-bits
        static Index xor_rand(PlyCount, const Index*);  // xor with random numbers conditional on a ply count
        static Index xor_rand(bool, Index);             // xor with a random number conditional on a boolean

        static const Index PIECES[2][64];
        static const Index KINGS[64];
        static const Index SIDE;
        static const Index SAME_KING[2][64];
        static const Index SAME_KING_MOVES[2][8];
};

}       // namespace zobrist
}       // namespace hash

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Random.hpp"