#include <stdint.h>
#include "Mix.h"

namespace dtl {
namespace hash {
namespace jenkins {

template<> const uint64_t Mix<uint64_t>::SIDE = 0xd2d84a61;
template<> const size_t Mix<uint64_t>::ADD_SHIFT_L[NUM_MIX] = {25, 4,  8, 26};
template<> const size_t Mix<uint64_t>::XOR_SHIFT_R[NUM_MIX] = {23, 6, 10, 31};

}       // namespace jenkins
}       // namespace hash
}       // namespace dtl
