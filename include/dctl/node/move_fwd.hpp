#pragma once
#include <cstdint>
#include <dctl/bit/bit_set.hpp>

namespace dctl {

template<class>
struct Move_;

using Move = Move_< bit::bit_set<int, uint64_t, 1> >;

}       // namespace dctl
