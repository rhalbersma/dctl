#pragma once
#include <dctl/bit/intrinsic.hpp>       // popcount

namespace dctl {
namespace bit {

// set.empty()
template<class T>
bool empty(T b)
{
        return b == T{0};
}

// set.size()
template<class T>
int size(T b)
{
        return bit::popcount(b);
}

template<class T>
bool raw_set_exclusive(T a, T b)
{
        return (a & b) == T{0};
}

template<class T>
bool raw_set_includes(T L, T R)
{
        return (R & ~L) == T{0};
}

}       // namespace bit
}       // namespace dctl
