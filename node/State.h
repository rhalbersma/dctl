#pragma once
#include "../utility/IntegerTypes.h"

namespace dctl {

template<typename> class Position;

template<typename Board> int state(const Position<Board>&);
template<bool, typename Board> int state(const Position<Board>&);
int state(bool, BitBoard, BitBoard);

}       // namespace dctl

// include template definitions inside header
#include "State.hpp"
