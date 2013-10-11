#pragma once
#include <limits>                               // digits
#include <dctl/bit/intrinsic/builtin.hpp>       // unchecked_ctz, unchecked_clz, popcount using gcc built-in functions
#include <dctl/bit/intrinsic/lookup.hpp>        // unchecked_ctz, unchecked_clz, popcount using table lookup
#include <dctl/bit/intrinsic/loop.hpp>          // unchecked_ctz, unchecked_clz, popcount using brute-force looping

namespace dctl {
namespace bit {

namespace lib = builtin;
using lib::unchecked_clz;
using lib::unchecked_ctz;
using lib::popcount;

template<class T>
constexpr auto bsf(T x)
{
        return unchecked_ctz(x);
}

template<class T>
constexpr auto bsr(T x)
{
        return std::numeric_limits<T>::digits - 1 - unchecked_clz(x);
}

template<class T>
constexpr auto ctz(T x) noexcept
{
        return x ? unchecked_ctz(x) : std::numeric_limits<T>::digits;
}

template<class T>
constexpr auto clz(T x) noexcept
{
        return x ? unchecked_clz(x) : std::numeric_limits<T>::digits;
}

}       // namespace bit
}       // namespace dctl
