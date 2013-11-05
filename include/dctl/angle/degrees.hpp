#pragma once
#include <dctl/angle/angle.hpp>
#include <dctl/preprocessor/static_constexpr_variable.hpp>      // DCTL_PP_STATIC_CONSTEXPR_VARIABLE

namespace dctl {

inline
constexpr auto operator"" _deg(unsigned long long n)
{
        return Angle{static_cast<int>(n)};
}

}       // namespace dctl
