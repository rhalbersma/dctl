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

inline
constexpr auto inverse2(Angle const& alpha) noexcept
{
        return -alpha;
}

inline
constexpr auto rotate2(Angle const& alpha, Angle const& theta) noexcept
{
        return alpha + theta;
}

inline
constexpr auto mirror2(Angle const& alpha, Angle const& theta) noexcept
{
        return rotate2(inverse2(rotate2(alpha,inverse2(theta))),theta);
}

}       // namespace angle
}       // namespace dctl
