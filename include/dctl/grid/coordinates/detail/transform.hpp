#pragma once

namespace dctl {
namespace grid {
namespace detail {

template<class T>
constexpr auto swap_llo_ulo(T const& v, T const& c) noexcept
{
        return (c - 1) - v;
}

template<class T>
constexpr auto sco_from_ulo(T const& v, T const& c) noexcept
{
        return 2 * v - (c - 1);
}

template<class T>
constexpr auto ulo_from_sco(T const& v, T const& c) noexcept
{
        return (v + (c - 1)) / 2;
}

}       // namespace detail
}       // namespace grid
}       // namespace dctl
