#pragma once
#include <dctl/angle/angle.hpp>
#include <dctl/preprocessor/static_constexpr_variable.hpp>      // DCTL_PP_STATIC_CONSTEXPR_VARIABLE

namespace dctl {

inline
constexpr int operator"" _deg(unsigned long long alpha)
{
    return static_cast<int>(alpha);
}

namespace angle {

/*

        Degrees are denoted as <DXXX>, where XXX is exactly three characters
        long, running from 000 until 360 in steps of 45. Counterclockwise and
        clockwise rotations are denoted as <LXXX> and <RXXX>, respectively,
        where XXX runs from 0 until 180.

                           D090 = L090
                                |
                   L135 = D135  |  D045 = L045
                              \ | /
                               \|/
        L180 = R180 = D180 ----- ----- D000 = D360 = L000 = R000
                               /|\
                              / | \
                   R135 = D225  |  D315 = R045
                                |
                           D270 = R090

*/

// named angles

DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, D000,   0)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, D045,  45)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, D090,  90)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, D135, 135)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, D180, 180)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, D225, 225)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, D270, 270)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, D315, 315)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, D360, 360)

// named counterclockwise (left) angles

DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, L000,   0)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, L045,  45)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, L090,  90)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, L135, 135)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, L180, 180)

// named clockwise (right) angles

DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, R180, 180)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, R135, 225)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, R090, 270)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, R045, 315)
DCTL_PP_STATIC_CONSTEXPR_VARIABLE(int, R000,   0)

}       // namespace angle
}       // namespace dctl
