#pragma once
#include <limits>                               // digits
#include <dctl/bit/intrinsic/builtin.hpp>       // ctznz, clznz, ctz, clz, popcount using gcc built-in functions
#include <dctl/bit/intrinsic/lookup.hpp>        // ctznz, clznz, ctz, clz, popcount using table lookup
#include <dctl/bit/intrinsic/loop.hpp>          // ctznz, clznz, ctz, clz, popcount using brute-force looping

namespace dctl {
namespace bit {

namespace lib = builtin;
using lib::ctznz;
using lib::clznz;
using lib::ctz;
using lib::clz;
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

}       // namespace bit
}       // namespace dctl
