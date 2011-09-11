#pragma once
#include "../../utility/IntegerTypes.h"

namespace dctl {
namespace hash {
namespace jenkins {

template<typename Index>
class Mix
{
public:
        static void mix(Index&, bool);                  // xor with a random number conditional on the boolean
	static void mix(Index&, BitBoard);              // apply a sequence of bitwise shifts

private:
        static void add_shift_L(Index&, int, BitBoard); 
        static void add_shift_L(Index&, int);
        static void xor_shift_R(Index&, int);

        static const Index SIDE;                        // random number used in the mixing with the side to move
        static const int NUM_MIX = 4;                   // number of mixing steps
        static const int ADD_SHIFT_L[NUM_MIX];          // shift lengths used in the addition steps
        static const int XOR_SHIFT_R[NUM_MIX];          // shift lengths used in the exclusive-OR steps
};

}       // namespace jenkins
}       // namespace hash
}       // namespace dctl

// include template definitions inside header
#include "Mix.hpp"
