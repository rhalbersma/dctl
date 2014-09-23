#pragma once
#include <dctl/angle/angle.hpp>

namespace dctl {

inline
constexpr auto operator"" _deg(unsigned long long n) noexcept
{
        return Angle{static_cast<int>(n)};
}

}       // namespace dctl
