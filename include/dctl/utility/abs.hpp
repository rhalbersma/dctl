#pragma once
#include<utility>                       // pair

namespace dctl {
namespace util {

template<class T>
constexpr auto is_negative(T const& value) noexcept
{
        return value < 0;
}

template<class T>
constexpr auto is_positive(T const& value) noexcept
{
        return 0 < value;
}

template<class T>
constexpr auto is_bounded(T const& value, std::pair<T, T> const& range) noexcept
{
        return range.first <= value && value < range.second;
}

template<class T>
constexpr auto abs(T const& value) noexcept
{
        return util::is_negative(value)? -value : value;
}

template<class T>
constexpr auto abs_remainder(T const& a, T const& b) noexcept
{
        // C++ Standard [expr.mul]/4: (a/b)*b + a%b is equal to a
        // NOTE: this implies that the sign of a%b is equal to the sign of a
        return a % b + (util::is_negative(a % b)? util::abs(b) : T{0});
}

}       // util
}       // dctl
