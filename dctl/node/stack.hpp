#pragma once
#include <vector>                       // vector
#include <dctl/node/move.hpp>
#include <dctl/guarded/arena_allocator.hpp>

namespace dctl {

#define DCTL_PP_VECTOR_RESERVE 16

typedef arena<DCTL_PP_VECTOR_RESERVE * sizeof(Move)> MoveArena;
typedef short_alloc<Move, DCTL_PP_VECTOR_RESERVE * sizeof(Move)> MoveAlloc;
typedef std::vector<Move, MoveAlloc> Stack;

typedef arena<DCTL_PP_VECTOR_RESERVE * sizeof(int)> IntArena;
typedef short_alloc<int, DCTL_PP_VECTOR_RESERVE * sizeof(int)> IntAlloc;
typedef std::vector<int, IntAlloc>  Variation;
typedef std::vector<int, IntAlloc>  Order;

}       // namespace dctl
