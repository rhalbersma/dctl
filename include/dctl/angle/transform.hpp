#pragma once
#include <dctl/angle/angle.hpp>         // make_angle

namespace dctl {
namespace angle {

template<class T>
constexpr auto inverse(T const& alpha) noexcept
{
        return dctl::make_angle(-alpha);
}

template<class T>
constexpr auto rotate(T const& alpha, T const& theta) noexcept
{
        return dctl::make_angle(alpha + theta);
}

template<class T>
constexpr auto mirror(T const& alpha, T const& theta) noexcept
{
        return angle::rotate(
                angle::inverse(
                        angle::rotate(
                                alpha,
                                angle::inverse(theta)
                        )
                ),
                theta
        );
}

}       // namespace angle
}       // namespace dctl
