#include <stdint.h>
#include "Mix.h"

template<> const uint64_t hash::jenkins::Mix<uint64_t>::SIDE = 0xd2d84a61;
template<> const size_t hash::jenkins::Mix<uint64_t>::ADD_SHIFT_L[NUM_MIX] = {25, 4,  8, 26};
template<> const size_t hash::jenkins::Mix<uint64_t>::XOR_SHIFT_R[NUM_MIX] = {23, 6, 10, 31};
