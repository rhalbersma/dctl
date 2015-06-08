#pragma once
#include <dctl/angle/angle.hpp> // Angle

namespace dctl {

constexpr auto inverse(Angle a) noexcept
{
        return -a;
}

constexpr auto rotate(Angle a, Angle t) noexcept
{
        return a + t;
}

constexpr auto mirror(Angle a, Angle t) noexcept
{
        // a | rotate(inverse(t)) | inverse() | rotate(t)
        return rotate(inverse(rotate(a, inverse(t))), t);
}

}       // namespace dctl
