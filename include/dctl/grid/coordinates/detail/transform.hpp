#pragma once

namespace dctl {
namespace grid {
namespace detail {

template<class T>
constexpr auto swap_llo_ulo(T const& value, T const& center) noexcept
{
        return (center - 1) - value;
}

template<class T>
constexpr auto sco_from_ulo(T const& value, T const& center) noexcept
{
        return 2 * value - (center - 1);
}

template<class T>
constexpr auto ulo_from_sco(T const& value, T const& center) noexcept
{
        return (value + (center - 1)) / 2;
}

}       // namespace detail
}       // namespace grid
}       // namespace dctl
