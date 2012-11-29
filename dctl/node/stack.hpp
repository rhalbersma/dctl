#pragma once
#include <vector>                       // vector
#include <dctl/guarded/short_alloc.hpp>
#include <dctl/node/move.hpp>

namespace dctl {

#define DCTL_PP_VECTOR_RESERVE 1024

#define SHORT_ALLOC 1

#if SHORT_ALLOC == 1

static auto const MoveMem = DCTL_PP_VECTOR_RESERVE * sizeof(Move);
static auto const IntMem = DCTL_PP_VECTOR_RESERVE * sizeof(int);

typedef stack_allocator<Move, MoveMem> MoveAlloc;
typedef stack_allocator<int, IntMem> IntAlloc;

typedef arena<MoveMem> MoveArena;
typedef arena<IntMem> IntArena;

typedef std::vector<Move, MoveAlloc> Stack;
typedef std::vector<int, IntAlloc> Variation;
typedef std::vector<int, IntAlloc> Order;

#else

typedef std::vector<Move> Stack;
typedef std::vector<int>  Variation;
typedef std::vector<int>  Order;

#endif

}       // namespace dctl
