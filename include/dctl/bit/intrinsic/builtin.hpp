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
struct ctz<uint32_t>
{
        static_assert(sizeof(uint32_t) == sizeof(unsigned), "");
        constexpr auto operator()(uint32_t x) const
        {
                assert(x != 0);
                return __builtin_ctz(x);
        }
};

template<>
struct ctz<uint64_t>
{
        static_assert(sizeof(uint64_t) == sizeof(unsigned long long), "");
        constexpr auto operator()(uint64_t x) const
        {
                assert(x != 0);
                return __builtin_ctzll(x);
        }
};

template<class T>
struct clz;

template<>
struct clz<uint32_t>
{
        static_assert(sizeof(uint32_t) == sizeof(unsigned), "");
        constexpr auto operator()(uint32_t x) const
        {
                assert(x != 0);
                return __builtin_clz(x);
        }
};

template<>
struct clz<uint64_t>
{
        static_assert(sizeof(uint64_t) == sizeof(unsigned long long), "");
        constexpr auto operator()(uint64_t x) const
        {
                assert(x != 0);
                return __builtin_clzll(x);
        }
};

template<class T>
struct popcount;

template<>
struct popcount<uint32_t>
{
        static_assert(sizeof(uint32_t) == sizeof(unsigned), "");
        constexpr auto operator()(uint32_t x) const noexcept
        {
                return __builtin_popcount(x);
        }
};

template<>
struct popcount<uint64_t>
{
        static_assert(sizeof(uint64_t) == sizeof(unsigned long long), "");
        constexpr auto operator()(uint64_t x) const noexcept
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
