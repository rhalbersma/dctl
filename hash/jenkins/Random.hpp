#pragma once
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace hash {
namespace jenkins {

template
<
        typename Index = uint64_t
>
struct Random
{
public:
        // xor with a random number conditional on the boolean
        static void mix(Index& index, bool color)
        {
                index = (0 - color) & SIDE;
        }

        // apply a sequence of bitwise shifts
        static void mix(Index& index, BitBoard b)
        {
                add_shift_L(index, ADD_SHIFT_L[0], b);
                xor_shift_R(index, XOR_SHIFT_R[0]);
                for (auto i = 1; i < NUM_MIX; ++i) {
                        add_shift_L(index, ADD_SHIFT_L[i]);
                        xor_shift_R(index, XOR_SHIFT_R[i]);
                }
        }

private:
        static void add_shift_L(Index& index, int s, BitBoard b)
        {
                index += (b << s) - b;
        }

        static void add_shift_L(Index& index, int s)
        {
                index += (index << s);
        }

        static void xor_shift_R(Index& index, int s)
        {
                index ^= (index >> s);
        }

        static const Index SIDE;                        // random number used in the mixing with the side to move
        static const int NUM_MIX = 4;                   // number of mixing steps
        static const int ADD_SHIFT_L[NUM_MIX];          // shift lengths used in the addition steps
        static const int XOR_SHIFT_R[NUM_MIX];          // shift lengths used in the exclusive-OR steps
};

template<typename Index>
const Index Random<Index>::SIDE = 0xd2d84a61;

template<typename Index>
const int Random<Index>::ADD_SHIFT_L[NUM_MIX] = { 25, 4,  8, 26 };

template<typename Index>
const int Random<Index>::XOR_SHIFT_R[NUM_MIX] = { 23, 6, 10, 31 };

// explicit instantation
template struct Random<>;

}       // namespace jenkins
}       // namespace hash
}       // namespace dctl
