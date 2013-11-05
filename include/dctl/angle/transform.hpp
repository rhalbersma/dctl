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
constexpr auto rotate(int a, Angle const& theta) noexcept
{
        return rotate(Angle{a}, theta);
}

inline
constexpr auto rotate(Angle const& alpha, int t) noexcept
{
        return rotate(alpha, Angle{t});
}

inline
constexpr auto mirror(Angle const& alpha, Angle const& theta) noexcept
{
        return rotate(inverse(rotate(alpha, inverse(theta))), theta);
}

inline
constexpr auto mirror(int a, Angle const& theta) noexcept
{
        return mirror(Angle{a}, theta);
}

inline
constexpr auto mirror(Angle const& alpha, int t) noexcept
{
        return mirror(alpha, Angle{t});
}

}       // namespace dctl
