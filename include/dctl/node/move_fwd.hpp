#pragma once
#include <dctl/bit/bitboard.hpp>        // BitBoard

namespace dctl {

template<class>
struct Move_;

using Move = Move_<BitBoard>;

}       // namespace dctl
