#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <dctl/utility/int.hpp>         // num_bits

namespace dctl {
namespace bit {
namespace loop {
namespace detail {

template<class T>
struct front
{
        int operator()(T b) const
        {
                for (auto i = 0; i < num_bits<T>::value; ++i)
                        if (b & (T{1} << i))
                                return i;
                BOOST_ASSERT(false);
                return num_bits<T>::value;
        }
};

template<class T>
struct back
{
        int operator()(T b) const
        {
                for (auto i = num_bits<T>::value - 1; i >= 0; --i)
                        if (b & (T{1} << i))
                                return i;
                BOOST_ASSERT(false);
                return num_bits<T>::value;
        }
};

template<class T>
struct size
{
        int operator()(T b) const
        {
                // Kernighan & Ritchie, The C programming language, 2nd Ed.
                // https://chessprogramming.wikispaces.com/Population+Count
                auto n = 0;
                for (; b; b &= b - T{1})
                        ++n;
                return n;
        }
};

}       // namespace detail

template<class T>
int front(T b)
{
        return detail::front<T>()(b);
}

template<class T>
int back(T b)
{
        return detail::back<T>()(b);
}

template<class T>
int size(T b)
{
        return detail::size<T>()(b);
}

}       // namespace loop
}       // namespace bit
}       // namespace dctl
