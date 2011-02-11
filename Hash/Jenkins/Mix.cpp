#include "Mix.h"
#include <cstdint>

const uint64_t Hash::Jenkins::Mix<uint64_t>::SIDE = 0xd2d84a61;
const size_t Hash::Jenkins::Mix<uint64_t>::ADD_SHIFT_L[NUM_MIX] = {25, 4,  8, 26};
const size_t Hash::Jenkins::Mix<uint64_t>::XOR_SHIFT_R[NUM_MIX] = {23, 6, 10, 31};
