#pragma once
#include <dctl/angle/angle.hpp>         // Angle

namespace dctl {

inline
constexpr auto inverse(Angle const& alpha) noexcept
{
        return -alpha;
}

inline
constexpr auto rotate(Angle const& alpha, Angle const& theta) noexcept
{
        return alpha + theta;
}

inline
constexpr auto mirror(Angle const& alpha, Angle const& theta) noexcept
{
        return rotate(inverse(rotate(alpha, inverse(theta))), theta);
}

}       // namespace dctl
