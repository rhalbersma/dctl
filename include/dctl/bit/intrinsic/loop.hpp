#pragma once
#include <dctl/bit/traits.hpp>  // one, digits
#include <cassert>              // assert

namespace dctl {
namespace bit {
namespace loop {

template<class T>
constexpr auto ctz(T x) noexcept
{
        for (auto i = 0; i < digits<T>; ++i)
                if (x & (one<T> << i))
                        return i;
        return digits<T>;
}

template<class T>
constexpr auto clz(T x) noexcept
{
        for (auto i = digits<T> - 1; i >= 0; --i)
                if (x & (one<T> << i))
                        return digits<T> - 1 - i;
        return digits<T>;
}

template<class T>
constexpr auto ctznz(T x) noexcept
{
        return ctz(x);
}

template<class T>
constexpr auto clznz(T x) noexcept
{
        return clz(x);
}

template<class T>
constexpr auto bsfnz(T x) noexcept
{
        return ctznz(x);
}

template<class T>
constexpr auto bsrnz(T x) noexcept
{
        return digits<T> - 1 - clznz(x);
}

template<class T>
constexpr auto popcount(T x) noexcept
{
        auto n = 0;
        for (; x; x &= x - one<T>)
                ++n;
        assert(0 <= n && n <= digits<T>);
        return n;
}

}       // namespace loop
}       // namespace bit
}       // namespace dctl
