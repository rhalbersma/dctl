#pragma once
#include <cstdint>
#include <dctl/bit/bit_set.hpp>

namespace dctl {

template<class>
struct Move_;

using Move = Move_< BitSet >;

}       // namespace dctl
