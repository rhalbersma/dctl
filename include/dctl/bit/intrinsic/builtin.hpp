#pragma once
#include <cassert>                      // assert
#include <limits>                       // digits
#include <type_traits>                  // enable_if

namespace dctl {
namespace bit {
namespace builtin {
namespace detail {

// gcc has built-in functions for trailing zero count
// for unsigned, unsigned long and unsigned long long
// for zero input, the result is undefined

template<class T>
struct ctznz;

template<>
struct ctznz<unsigned>
{
        constexpr auto operator()(unsigned x) const noexcept
        {
                return __builtin_ctz(x);
        }
};

template<>
struct ctznz<unsigned long>
{
        constexpr auto operator()(unsigned long x) const noexcept
        {
                return __builtin_ctzl(x);
        }
};

template<>
struct ctznz<unsigned long long>
{
        constexpr auto operator()(unsigned long long x) const noexcept
        {
                return __builtin_ctzll(x);
        }
};

// gcc has built-in functions for leading zero count
// for unsigned, unsigned long and unsigned long long
// for zero input, the result is undefined

template<class T>
struct clznz;

template<>
struct clznz<unsigned>
{
        constexpr auto operator()(unsigned x) const noexcept
        {
                return __builtin_clz(x);
        }
};

template<>
struct clznz<unsigned long>
{
        constexpr auto operator()(unsigned long x) const noexcept
        {
                return __builtin_clzl(x);
        }
};

template<>
struct clznz<unsigned long long>
{
        constexpr auto operator()(unsigned long long x) const noexcept
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
constexpr auto ctznz(T x) noexcept
{
        assert(x != 0);
        return detail::ctznz<T>()(x);
}

template<class T>
constexpr auto clznz(T x) noexcept
{
        assert(x != 0);
        return detail::clznz<T>()(x);
}

template<class T>
constexpr auto ctz(T x) noexcept
{
        return x ? ctznz(x) : std::numeric_limits<T>::digits;
}

template<class T>
constexpr auto clz(T x) noexcept
{
        return x ? clznz(x) : std::numeric_limits<T>::digits;
}

template<class T>
constexpr auto popcount(T x) noexcept
{
        return detail::popcount<T>()(x);
}

}       // namespace builtin
}       // namespace bit
}       // namespace dctl
