#pragma once
#include <dctl/utility/int.hpp>         // num_bits
#include <dctl/bit/intrinsic.hpp>
#include <dctl/bit/algorithm.hpp>

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
        b &= b - T(1);
}

template<class T>
bool empty(T b)
{
        return b == T(0);
}

template<class T>
int size(T b)
{
        return intrinsic::popcount(b);
}

template<class T>
int max_size(T)
{
        return num_bits<T>::value;
}

template<class T>
T minimal_element(T b)
{
        return b & (T(0) - b);
}

template<class T>
bool raw_set_exclusive(T a, T b)
{
        return (a & b) == 0;
}

template<class T>
bool raw_set_includes(T L, T R)
{
        return (R & ~L) == 0;
}

}       // namespace bit
}       // namespace dctl
