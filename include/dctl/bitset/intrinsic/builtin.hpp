#pragma once
#include <dctl/bitset/limits.hpp> // digits
#include <cassert>              // assert

namespace xstd {
namespace builtin {
namespace detail {

// GCC has built-in functions for Count Trailing Zeros
// for unsigned, unsigned long and unsigned long long.
// For zero input, the result is undefined.

template<class T>
struct ctznz;

template<>
struct ctznz<unsigned>
{
        constexpr auto operator()(unsigned x) const
        {
                return __builtin_ctz(x);
        }
};

template<>
struct ctznz<unsigned long>
{
        constexpr auto operator()(unsigned long x) const
        {
                return __builtin_ctzl(x);
        }
};

template<>
struct ctznz<unsigned long long>
{
        constexpr auto operator()(unsigned long long x) const
        {
                return __builtin_ctzll(x);
        }
};

// GCC has built-in functions for Count Leading Zeros
// for unsigned, unsigned long and unsigned long long.
// For zero input, the result is undefined

template<class T>
struct clznz;

template<>
struct clznz<unsigned>
{
        constexpr auto operator()(unsigned x) const
        {
                return __builtin_clz(x);
        }
};

template<>
struct clznz<unsigned long>
{
        constexpr auto operator()(unsigned long x) const
        {
                return __builtin_clzl(x);
        }
};

template<>
struct clznz<unsigned long long>
{
        constexpr auto operator()(unsigned long long x) const
        {
                return __builtin_clzll(x);
        }
};

// GCC has built-in functions for Population Count
// for unsigned, unsigned long and unsigned long long.

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
constexpr auto ctznz(T x)
{
        assert(x != 0);
        return detail::ctznz<T>{}(x);
}

template<class T>
constexpr auto clznz(T x)
{
        assert(x != 0);
        return detail::clznz<T>{}(x);
}

template<class T>
constexpr auto ctz(T x) noexcept
{
        return x ? ctznz(x) : digits<T>;
}

template<class T>
constexpr auto clz(T x) noexcept
{
        return x ? clznz(x) : digits<T>;
}

template<class T>
constexpr auto bsfnz(T x)
{
        assert(x != 0);
        return ctznz(x);
}

template<class T>
constexpr auto bsrnz(T x)
{
        assert(x != 0);
        return digits<T> - 1 - clznz(x);
}

template<class T>
constexpr auto popcount(T x) noexcept
{
        return detail::popcount<T>{}(x);
}

}       // namespace builtin
}       // namespace xstd
