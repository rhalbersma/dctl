#pragma once
#include <vector>                               // vector
#include <dctl/utility/algorithm.hpp>           // contains
#include <dctl/utility/arena_allocator.hpp>     // arena, short_alloc

namespace dctl {

#define DCTL_PP_STACK_RESERVE 16

template<typename T>
struct Arena
{
        typedef arena<DCTL_PP_STACK_RESERVE * sizeof(T)> type;
};

template<typename T>
struct Alloc
{
        typedef short_alloc<T, DCTL_PP_STACK_RESERVE> type;
};

template<typename T>
struct stack_vector
{
        typedef std::vector<T, typename Alloc<T>::type > type;
};

typedef stack_vector<int>::type Variation;
typedef stack_vector<int>::type Order;

}       // namespace dctl
