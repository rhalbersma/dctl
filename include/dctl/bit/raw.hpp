#pragma once
#include <limits>                       // digits
#include <dctl/bit/intrinsic.hpp>       // ctz, popcount

namespace dctl {
namespace bit {

template<class T>
int front(T b)
{
        return intrinsic::ctz(b);
}

template<class T>
void pop_front(T& b)
{
        b &= b - T{1};
}

template<class T>
bool empty(T b)
{
        return b == T{0};
}

template<class T>
int size(T b)
{
        return intrinsic::popcount(b);
}

template<class T>
int max_size(T)
{
        return std::numeric_limits<T>::digits;
}

template<class T>
T minimal_element(T b)
{
        return b & (T{0} - b);
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
