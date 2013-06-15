#pragma once
#include <cstdint>                      // uint32_t, uint64_t
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include <dctl/utility/int.hpp>         // num_bits

namespace dctl {
namespace bit {
namespace intrinsic {
namespace detail {

template<class T>
struct front;

template<>
struct front<uint32_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint32_t>::value == num_bits<unsigned int>::value);
        int operator()(uint32_t b) const
        {
                return static_cast<int>(__builtin_ctz(b));
        }
};

template<>
struct front<uint64_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint64_t>::value == num_bits<unsigned long long int>::value);
        int operator()(uint64_t b) const
        {
                return static_cast<int>(__builtin_ctzll(b));
        }
};

template<class T>
struct back;

template<>
struct back<uint32_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint32_t>::value == num_bits<unsigned int>::value);
        int operator()(uint32_t b) const
        {
                return num_bits<uint32_t>::value - 1 - static_cast<int>(__builtin_clz(b));
        }
};

template<>
struct back<uint64_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint64_t>::value == num_bits<unsigned long long int>::value);
        int operator()(uint64_t b) const
        {
                return num_bits<uint64_t>::value - 1 - static_cast<int>(__builtin_clzll(b));
        }
};

template<class T>
struct size;

template<>
struct size<uint32_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint32_t>::value == num_bits<unsigned int>::value);
        int operator()(uint32_t b) const
        {
                return static_cast<int>(__builtin_popcount(b));
        }
};

template<>
struct size<uint64_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint64_t>::value == num_bits<unsigned long long int>::value);
        int operator()(uint64_t b) const
        {
                return static_cast<int>(__builtin_popcountll(b));
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

}       // namespace intrinsic
}       // namespace bit
}       // namespace dctl
