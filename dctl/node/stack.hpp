#pragma once
#include <vector>                       // vector
#include <dctl/node/move.hpp>
#include <dctl/guarded/arena_allocator.hpp>
#include <dctl/utility/algorithm.hpp>   // contains

namespace dctl {

#define DCTL_PP_VECTOR_RESERVE 16

template<typename T>
using Arena = arena<DCTL_PP_VECTOR_RESERVE * sizeof(T)>;

template<typename T>
using Alloc = short_alloc<T, DCTL_PP_VECTOR_RESERVE>;

template<typename T>
using Vector = std::vector<T, Alloc<T> >;

typedef Vector<int>  Variation;
typedef Vector<int>  Order;

template<typename Move>
bool is_duplicate_back(Vector<Move> const& moves)
{
        return algorithm::detect(std::begin(moves), std::end(moves) - 1, moves.back());
}

}       // namespace dctl
