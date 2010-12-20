#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename Index>
class JenkinsMix
{
public:
        static void mix(Index&, bool);                          // xor with a random number conditional on the boolean
	static void mix(Index&, BitBoard);                      // apply a sequence of bitwise shifts

private:
        static void add_shift_L(Index&, size_t, BitBoard);
        static void add_shift_L(Index&, size_t);
        static void xor_shift_R(Index&, size_t);

        static const Index SIDE;                                // random number used in the mixing with the side to move
        static const size_t NUM_MIX = 4;                        // number of mixing steps
        static const size_t ADD_SHIFT_L[NUM_MIX];               // shift lengths used in the addition steps
        static const size_t XOR_SHIFT_R[NUM_MIX];               // shift lengths used in the exclusive-OR steps
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "JenkinsMix.hpp"
