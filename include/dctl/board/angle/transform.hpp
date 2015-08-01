#pragma once
#include <dctl/board/angle/angle.hpp> // Angle

namespace dctl {

constexpr auto inverse(Angle const& a) noexcept
{
        return -a;
}

constexpr auto rotate(Angle const& a, Angle const& t) noexcept
{
        return a + t;
}

constexpr auto mirror(Angle const& a, Angle const& t) noexcept
{
        // a | rotate(inverse(t)) | inverse() | rotate(t)
        return rotate(inverse(rotate(a, inverse(t))), t);
}

}       // namespace dctl
