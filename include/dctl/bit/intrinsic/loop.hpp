#pragma once
#include <cassert>                      // assert
#include <limits>                       // digits

namespace dctl {
namespace bit {
namespace loop {
namespace detail {

template<class T>
struct ctz
{
        constexpr auto operator()(T x) const noexcept
        {

                for (auto i = 0; i < std::numeric_limits<T>::digits; ++i)
                        if (x & (T{1} << i))
                                return i;
                return std::numeric_limits<T>::digits;
        }
};

template<class T>
struct clz
{
        constexpr auto operator()(T x) const noexcept
        {
                for (auto i = std::numeric_limits<T>::digits - 1; i >= 0; --i)
                        if (x & (T{1} << i))
                                return std::numeric_limits<T>::digits - 1 - i;
                return std::numeric_limits<T>::digits;;
        }
};

template<class T>
struct popcount
{
        constexpr auto operator()(T x) const noexcept
        {
                auto n = 0;
                for (; x; x &= x - T{1})
                        ++n;
                assert(0 <= n && n <= std::numeric_limits<T>::digits);
                return n;
        }
};

}       // namespace detail

template<class T>
constexpr auto ctz(T x) noexcept
{
        return detail::ctz<T>()(x);
}

template<class T>
constexpr auto clz(T x) noexcept
{
        return detail::clz<T>()(x);
}

template<class T>
constexpr auto popcount(T x) noexcept
{
        return detail::popcount<T>()(x);
}

}       // namespace loop
}       // namespace bit
}       // namespace dctl
