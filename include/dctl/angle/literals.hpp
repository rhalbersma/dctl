#pragma once
#include <dctl/angle/angle.hpp>

namespace dctl {
inline namespace literals {
inline namespace angle_literals {

inline
constexpr auto operator"" _deg(unsigned long long n) noexcept
{
        return Angle{static_cast<int>(n)};
}

}       // namespace angle_literals
}       // namespace literals
}       // namespace dctl
