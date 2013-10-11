#pragma once
#include <cassert>                      // assert

namespace dctl {
namespace bit {
namespace builtin {
namespace detail {

// gcc has built-in functions for trailing zero count
// for unsigned, unsigned long and unsigned long long
// for zero input, the result is undefined

template<class T>
struct unchecked_ctz;

template<>
struct unchecked_ctz<unsigned>
{
        constexpr auto operator()(unsigned x) const
        {
                return __builtin_ctz(x);
        }
};

template<>
struct unchecked_ctz<unsigned long>
{
        constexpr auto operator()(unsigned long x) const
        {
                return __builtin_ctzl(x);
        }
};

template<>
struct unchecked_ctz<unsigned long long>
{
        constexpr auto operator()(unsigned long long x) const
        {
                return __builtin_ctzll(x);
        }
};

// gcc has built-in functions for leading zero count
// for unsigned, unsigned long and unsigned long long
// for zero input, the result is undefined

template<class T>
struct unchecked_clz;

template<>
struct unchecked_clz<unsigned>
{
        constexpr auto operator()(unsigned x) const
        {
                return __builtin_clz(x);
        }
};

template<>
struct unchecked_clz<unsigned long>
{
        constexpr auto operator()(unsigned long x) const
        {
                return __builtin_clzl(x);
        }
};

template<>
struct unchecked_clz<unsigned long long>
{
        constexpr auto operator()(unsigned long long x) const
        {
                return __builtin_clzll(x);
        }
};

// gcc has built-in functions for population count
// for unsigned, unsigned long and unsigned long long,

template<class T>
struct popcount;

template<>
struct popcount<unsigned>
{
        constexpr auto operator()(unsigned x) const noexcept
        {
                return __builtin_popcount(x);
        }
};

template<>
struct popcount<unsigned long>
{
        constexpr auto operator()(unsigned long x) const noexcept
        {
                return __builtin_popcountl(x);
        }
};

template<>
struct popcount<unsigned long long>
{
        constexpr auto operator()(unsigned long long x) const noexcept
        {
                return __builtin_popcountll(x);
        }
};

}       // namespace detail

template<class T>
constexpr auto unchecked_ctz(T x)
{
        assert(x != 0);
        return detail::unchecked_ctz<T>()(x);
}

template<class T>
constexpr auto unchecked_clz(T x)
{
        assert(x != 0);
        return detail::unchecked_clz<T>()(x);
}

template<class T>
constexpr auto popcount(T x) noexcept
{
        return detail::popcount<T>()(x);
}

}       // namespace builtin
}       // namespace bit
}       // namespace dctl
