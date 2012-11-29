#pragma once
#include <vector>                       // vector
#include <dctl/guarded/short_alloc.hpp>
#include <dctl/node/move.hpp>

namespace dctl {

#define DCTL_PP_VECTOR_RESERVE 32

#define SHORT_ALLOC 1

#if SHORT_ALLOC == 1

typedef short_alloc<Move, DCTL_PP_VECTOR_RESERVE * sizeof(Move)> MoveAlloc;
typedef short_alloc<int, DCTL_PP_VECTOR_RESERVE * sizeof(int)> IntAlloc;

typedef arena<DCTL_PP_VECTOR_RESERVE * sizeof(Move)> MoveArena;
typedef arena<DCTL_PP_VECTOR_RESERVE * sizeof(int)> IntArena;

typedef std::vector<Move, MoveAlloc> Stack;
typedef std::vector<int, IntAlloc> Variation;
typedef std::vector<int, IntAlloc> Order;

#else

typedef std::vector<Move> Stack;
typedef std::vector<int>  Variation;
typedef std::vector<int>  Order;

#endif

}       // namespace dctl
