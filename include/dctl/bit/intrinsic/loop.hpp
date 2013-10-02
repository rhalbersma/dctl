#pragma once
#include <cassert>                      // assert
#include <dctl/utility/int.hpp>         // num_bits

namespace dctl {
namespace bit {
namespace loop {
namespace detail {

template<class T>
struct ctz
{
        int operator()(T x) const
        {
                for (auto i = 0; i < num_bits<T>::value; ++i)
                        if (x & (T{1} << i))
                                return i;
                return num_bits<T>::value;
        }
};

template<class T>
struct clz
{
        int operator()(T x) const
        {
                for (auto i = num_bits<T>::value - 1; i >= 0; --i)
                        if (x & (T{1} << i))
                                return num_bits<T>::value - 1 - i;
                return num_bits<T>::value;
        }
};

template<class T>
struct popcount
{
        int operator()(T x) const
        {
                auto n = 0;
                for (; x; x &= x - T{1})
                        ++n;
                assert(0 <= n && n <= num_bits<T>::value);
                return n;
        }
};

}       // namespace detail

template<class T>
int ctz(T x)
{
        return detail::ctz<T>()(x);
}

template<class T>
int clz(T x)
{
        return detail::clz<T>()(x);
}

template<class T>
int popcount(T x)
{
        return detail::popcount<T>()(x);
}

}       // namespace loop
}       // namespace bit
}       // namespace dctl
