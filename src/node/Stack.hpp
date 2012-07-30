#pragma once
#include <vector>                       // vector
#include "Move.hpp"
#include "../utility/stack_alloc.hpp"

namespace dctl {

#define SET_USE_STACK_ALLOC 0
#define MOVE_RESERVE 32

#if (_MSC_VER <= 1600) && _DEBUG
#define USE_STACK_ALLOC 0
#else
#define USE_STACK_ALLOC SET_USE_STACK_ALLOC
#endif

#if USE_STACK_ALLOC == 1
        typedef std::vector<Move, stack_alloc<Move, MOVE_RESERVE> > Stack;
#else
        typedef std::vector<Move> Stack;
#endif 

}       // namespace dctl
