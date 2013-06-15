#pragma once
#include <dctl/utility/int.hpp>         // num_bits
#include <dctl/bit/intrinsic.hpp>
#include <dctl/bit/lookup.hpp>
#include <dctl/bit/loop.hpp>

namespace dctl {
namespace bit {

template<class T>
int front(T b)
{
        return intrinsic::front(b);
}

template<class T>
void pop_front(T& b)
{
        b &= b - T(1);
}

template<class T>
int back(T b)
{
        return intrinsic::back(b);
}

template<class T>
void pop_back(T& b)
{
        b ^= T(1) << back(b);
}

template<class T>
bool empty(T b)
{
        return b == T(0);
}

template<class T>
int size(T b)
{
        return intrinsic::size(b);
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

}       // namespace bit
}       // namespace dctl
