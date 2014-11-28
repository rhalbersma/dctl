#pragma once
#include <dctl/utility/arena_allocator.hpp>     // arena, short_alloc
#include <vector>                               // vector

namespace dctl {

#define DCTL_PP_STACK_RESERVE 16

template<class T, std::size_t N = DCTL_PP_STACK_RESERVE>
using Arena = arena<N * sizeof(T)>;

template<class T, std::size_t N = DCTL_PP_STACK_RESERVE>
using Alloc = short_alloc<T, N>;

template<class T, std::size_t N = DCTL_PP_STACK_RESERVE>
using stack_vector = std::vector< T, Alloc<T, N> >;

using Variation = stack_vector<int>;
using Order = stack_vector<int>;

}       // namespace dctl
