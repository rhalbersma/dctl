#pragma once
#include <limits>                               // digits
#include <dctl/bit/intrinsic/builtin.hpp>       // ctznz, clznz, popcount using gcc built-in functions
#include <dctl/bit/intrinsic/lookup.hpp>        // ctznz, clznz, popcount using table lookup
#include <dctl/bit/intrinsic/loop.hpp>          // ctznz, clznz, popcount using brute-force looping

namespace dctl {
namespace bit {

namespace lib = builtin;
using lib::clznz;
using lib::ctznz;
using lib::popcount;

template<class T>
constexpr auto bsf(T x)
{
        return ctznz(x);
}

template<class T>
constexpr auto bsr(T x)
{
        return std::numeric_limits<T>::digits - 1 - clznz(x);
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

}       // namespace bit
}       // namespace dctl
