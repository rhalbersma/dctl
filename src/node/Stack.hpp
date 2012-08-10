#pragma once
#include <vector>                       // vector
#include "Move.hpp"
#include "../utility/stack_alloc.hpp"

namespace dctl {

#if (_MSC_VER <= 1600) && _DEBUG
#define USE_STACK_ALLOC 0
#else
#define USE_STACK_ALLOC 1
#endif

#define MOVE_RESERVE 32

#if USE_STACK_ALLOC == 1
        typedef std::vector<Move, stack_alloc<Move, MOVE_RESERVE> > Stack;
        typedef std::vector<int,  stack_alloc<int,  MOVE_RESERVE> > Variation;
        typedef std::vector<int,  stack_alloc<int,  MOVE_RESERVE> > Order;
#else
        typedef std::vector<Move> Stack;
        typedef std::vector<int>  Variation;
        typedef std::vector<int>  Order;
#endif

}       // namespace dctl
