#pragma once
#include <cassert>                      // assert
#include <cstdint>                      // uint32_t, uint64_t, CHAR_BIT

namespace dctl {
namespace bit {
namespace intrinsic {
namespace detail {

template<class T>
struct ctz;

template<>
struct ctz<uint32_t>
{
        static_assert(sizeof(uint32_t) == sizeof(unsigned), "");
        constexpr auto operator()(uint32_t b) const
        {
                assert(b != 0);
                return __builtin_ctz(b);
        }
};

template<>
struct ctz<uint64_t>
{
        static_assert(sizeof(uint64_t) == sizeof(unsigned long long), "");
        constexpr auto operator()(uint64_t b) const
        {
                assert(b != 0);
                return __builtin_ctzll(b);
        }
};

template<class T>
struct clz;

template<>
struct clz<uint32_t>
{
        static_assert(sizeof(uint32_t) == sizeof(unsigned), "");
        constexpr auto operator()(uint32_t b) const
        {
                assert(b != 0);
                return __builtin_clz(b);
        }
};

template<>
struct clz<uint64_t>
{
        static_assert(sizeof(uint64_t) == sizeof(unsigned long long), "");
        constexpr auto operator()(uint64_t b) const
        {
                assert(b != 0);
                return __builtin_clzll(b);
        }
};

template<class T>
struct popcount;

template<>
struct popcount<uint32_t>
{
        static_assert(sizeof(uint32_t) == sizeof(unsigned), "");
        constexpr auto operator()(uint32_t b) const noexcept
        {
                return __builtin_popcount(b);
        }
};

template<>
struct popcount<uint64_t>
{
        static_assert(sizeof(uint64_t) == sizeof(unsigned long long), "");
        constexpr auto operator()(uint64_t b) const noexcept
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
constexpr auto popcount(T b) noexcept
{
        return detail::popcount<T>()(b);
}

template<class T>
constexpr auto front(T b)
{
        return intrinsic::ctz(b);
}

template<class T>
constexpr auto back(T b)
{
        return static_cast<int>(CHAR_BIT * sizeof(T)) - 1 - intrinsic::clz(b);
}

template<class T>
constexpr auto size(T b) noexcept
{
        return detail::popcount<T>()(b);
}

}       // namespace intrinsic
}       // namespace bit
}       // namespace dctl
