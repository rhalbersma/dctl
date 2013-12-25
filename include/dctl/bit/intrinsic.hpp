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

// Bit Scan Forward for Non Zero input
template<class T>
constexpr auto bsfnz(T x) noexcept
{
        return ctznz(x);
}

// Bit Scan Reverse for Non Zero input
template<class T>
constexpr auto bsrnz(T x) noexcept
{
        return std::numeric_limits<T>::digits - 1 - clznz(x);
}

}       // namespace bit
}       // namespace dctl
