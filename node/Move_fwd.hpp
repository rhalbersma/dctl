#pragma once
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename>
struct Move_;

typedef Move_<BitBoard> Move;

}       // namespace dctl
