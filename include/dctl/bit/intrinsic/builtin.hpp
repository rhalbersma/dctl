#pragma once
#include <cassert>                      // assert
#include <cstdint>                      // uint32_t, uint64_t

namespace dctl {
namespace bit {
namespace builtin {
namespace detail {

template<class T>
struct ctz;

template<>
struct ctz<unsigned>
{
        constexpr auto operator()(unsigned x) const
        {
                assert(x != 0);
                return __builtin_ctz(x);
        }
};

template<>
struct ctz<unsigned long>
{
        constexpr auto operator()(unsigned long x) const
        {
                assert(x != 0);
                return __builtin_ctzl(x);
        }
};

template<>
struct ctz<unsigned long long>
{
        constexpr auto operator()(unsigned long long x) const
        {
                assert(x != 0);
                return __builtin_ctzll(x);
        }
};

template<class T>
struct clz;

template<>
struct clz<unsigned>
{
        constexpr auto operator()(unsigned x) const
        {
                assert(x != 0);
                return __builtin_clz(x);
        }
};

template<>
struct clz<unsigned long>
{
        constexpr auto operator()(unsigned long x) const
        {
                assert(x != 0);
                return __builtin_clzl(x);
        }
};

template<>
struct clz<unsigned long long>
{
        constexpr auto operator()(unsigned long long x) const
        {
                assert(x != 0);
                return __builtin_clzll(x);
        }
};

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
constexpr auto ctz(T x)
{
        return detail::ctz<T>()(x);
}

template<class T>
constexpr auto clz(T x)
{
        return detail::clz<T>()(x);
}

template<class T>
constexpr auto popcount(T x) noexcept
{
        return detail::popcount<T>()(x);
}

}       // namespace builtin
}       // namespace bit
}       // namespace dctl
