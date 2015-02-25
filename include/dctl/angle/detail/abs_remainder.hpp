#pragma once

namespace dctl {
namespace detail {

// abs(int) from <cstdlib> is not (yet) constexpr
inline
constexpr auto abs(int n) noexcept
{
        return (n < 0) ? -n : n;
}

inline
constexpr auto abs_remainder(int a, int b) noexcept
{
        // C++ Standard [expr.mul]/4: (a/b)*b + a%b == a
        // NOTE: this implies that sign(a%b) == sign(a)
        return a % b + (((a % b) < 0) ? detail::abs(b) : 0);
}

}       // namespace detail
}       // namespace dctl
