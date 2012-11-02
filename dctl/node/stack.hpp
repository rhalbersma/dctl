#pragma once
#include <vector>                       // vector
#include <dctl/node/move.hpp>

namespace dctl {

#define MOVE_RESERVE 32

typedef std::vector<Move> Stack;
typedef std::vector<int>  Variation;
typedef std::vector<int>  Order;

}       // namespace dctl
