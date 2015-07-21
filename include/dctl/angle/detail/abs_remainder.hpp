#pragma once
#include <cassert>      // assert

namespace dctl {
namespace detail {

// abs(int) from <cstdlib> is not (yet) constexpr
constexpr auto abs(int n) noexcept
{
        return (n < 0) ? -n : n;
}

constexpr auto abs_remainder(int a, int b) noexcept
{
        // C++ Standard [expr.mul]/4: (a/b)*b + a%b == a
        // NOTE: this implies that sign(a%b) == sign(a)
        auto const r = a % b + (((a % b) < 0) ? detail::abs(b) : 0);
        assert(0 <= r && r < b);
        return r;
}

}       // namespace detail
}       // namespace dctl
