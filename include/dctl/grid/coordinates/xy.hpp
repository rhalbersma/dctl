#pragma once

namespace dctl {
namespace grid {

template<class T>
constexpr auto get_x(T const& coordinates) noexcept
{
        return coordinates.first;
}

template<class T>
constexpr auto get_y(T const& coordinates) noexcept
{
        return coordinates.second;
}

}       // namespace grid
}       // namespace dctl
