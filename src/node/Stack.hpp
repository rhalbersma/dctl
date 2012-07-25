#pragma once
#include <vector>                       // vector
#include "Move.hpp"
#include "../utility/stack_alloc.hpp"

namespace dctl {

#define USE_STACK_ALLOC 1
#define MOVE_RESERVE 32

#if USE_STACK_ALLOC == 1
        typedef std::vector<Move, stack_alloc<Move, MOVE_RESERVE> > Stack;
#else
        typedef std::vector<Move> Stack;
#endif 

}       // namespace dctl
