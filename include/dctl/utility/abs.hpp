#pragma once

namespace dctl {
namespace util {

template<class T>
constexpr auto abs(T const& value) noexcept
{
        return (value < 0) ? -value : value;
}

template<class T>
constexpr auto abs_remainder(T const& a, T const& b) noexcept
{
        // C++ Standard [expr.mul]/4: (a/b)*b + a%b is equal to a
        // NOTE: this implies that the sign of a%b is equal to the sign of a
        return a % b + (((a % b) < 0)? util::abs(b) : T{0});
}

}       // util
}       // dctl
