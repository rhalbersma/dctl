#pragma once
#include <cassert>                      // assert
#include <limits>                       // digits
#include <stdexcept>                    // invalid argument

namespace dctl {
namespace bit {
namespace loop {

template<class T>
constexpr auto unchecked_ctz(T x)
{
        assert(x != 0);
        for (auto i = 0; i < std::numeric_limits<T>::digits; ++i)
                if (x & (T{1} << i))
                        return i;
        throw std::invalid_argument("unchecked_ctz requires non-zero argument");
}

template<class T>
constexpr auto unchecked_clz(T x)
{
        assert(x != 0);
        for (auto i = std::numeric_limits<T>::digits - 1; i >= 0; --i)
                if (x & (T{1} << i))
                        return std::numeric_limits<T>::digits - 1 - i;
        throw std::invalid_argument("unchecked_clz requires non-zero argument");
}

template<class T>
constexpr auto popcount(T x) noexcept
{
        auto n = 0;
        for (; x; x &= x - T{1})
                ++n;
        assert(0 <= n && n <= std::numeric_limits<T>::digits);
        return n;
}

}       // namespace loop
}       // namespace bit
}       // namespace dctl
