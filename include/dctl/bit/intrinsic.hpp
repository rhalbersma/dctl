#pragma once
#include <cstdint>                      // uint32_t, uint64_t
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include <dctl/utility/int.hpp>         // num_bits

namespace dctl {
namespace bit {
namespace intrinsic {
namespace detail {

template<class T>
struct ctz;

template<>
struct ctz<uint32_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint32_t>::value == num_bits<unsigned int>::value);
        constexpr auto operator()(uint32_t b) const
        {
                return __builtin_ctz(b);
        }
};

template<>
struct ctz<uint64_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint64_t>::value == num_bits<unsigned long long int>::value);
        constexpr auto operator()(uint64_t b) const
        {
                return __builtin_ctzll(b);
        }
};

template<class T>
struct clz;

template<>
struct clz<uint32_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint32_t>::value == num_bits<unsigned int>::value);
        constexpr auto operator()(uint32_t b) const
        {
                return __builtin_clz(b);
        }
};

template<>
struct clz<uint64_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint64_t>::value == num_bits<unsigned long long int>::value);
        constexpr auto operator()(uint64_t b) const
        {
                return __builtin_clzll(b);
        }
};

template<class T>
struct size;

template<>
struct size<uint32_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint32_t>::value == num_bits<unsigned int>::value);
        constexpr auto operator()(uint32_t b) const
        {
                return __builtin_popcount(b);
        }
};

template<>
struct size<uint64_t>
{
        BOOST_STATIC_ASSERT(num_bits<uint64_t>::value == num_bits<unsigned long long int>::value);
        constexpr auto operator()(uint64_t b) const
        {
                return __builtin_popcountll(b);
        }
};

}       // namespace detail

template<class T>
constexpr auto ctz(T b)
{
        return detail::ctz<T>()(b);
}

template<class T>
constexpr auto clz(T b)
{
        return detail::clz<T>()(b);
}

template<class T>
constexpr auto front(T b)
{
        return intrinsic::ctz(b);
}

template<class T>
constexpr auto back(T b)
{
        return num_bits<T>::value - 1 - intrinsic::clz(b);
}

template<class T>
constexpr auto size(T b)
{
        return detail::size<T>()(b);
}

}       // namespace intrinsic
}       // namespace bit
}       // namespace dctl
