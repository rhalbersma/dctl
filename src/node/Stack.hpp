#pragma once
#include <vector>                       // vector
#include "Move.hpp"

/*---------------------------------------------------------------------------*/
#if (_MSC_VER <= 1600)
#pragma warning(push)
#pragma warning(disable: 4100 ) // unreferenced formal parameter
#endif
/*---------------------------------------------------------------------------*/

#include "../utility/stack_alloc.h"

/*---------------------------------------------------------------------------*/
#if (_MSC_VER <= 1600)
#pragma warning(pop)
#endif
/*---------------------------------------------------------------------------*/

namespace dctl {

#define USE_STACK_ALLOC 1
#define MOVE_RESERVE 32

#if USE_STACK_ALLOC == 1
        typedef std::vector<Move, stack_alloc<Move, MOVE_RESERVE> > Stack;
#else
        typedef std::vector<Move> Stack;
#endif 

}       // namespace dctl
