#pragma once

namespace dctl {
namespace util {

template<class T>
constexpr auto abs(T const& t) noexcept
{
        return (t < 0)? -t : t;
}

template<class T>
constexpr auto is_abs(T const& t) noexcept
{
        return 0 <= t;
}

template<class T>
constexpr auto abs_modulus(T const& n, T const& d) noexcept
{
        return n % d + ((n % d < 0)? util::abs(d) : T{0});
}

template<class T>
constexpr auto is_abs_modulus(T const& n, T const& d) noexcept
{
        return 0 <= n && n < d;
}

}       // util
}       // dctl
