#pragma once
#include <limits>                                       // digits
#include <dctl/bit/detail/intrinsic/builtin.hpp>        // ctznz, clznz, ctz, clz, popcount using gcc built-in functions
#include <dctl/bit/detail/intrinsic/lookup.hpp>         // ctznz, clznz, ctz, clz, popcount using table lookup
#include <dctl/bit/detail/intrinsic/loop.hpp>           // ctznz, clznz, ctz, clz, popcount using brute-force looping

namespace dctl {
namespace bit {
namespace intrinsic {

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

}       // namespace intrinsic
}       // namespace bit
}       // namespace dctl
