#pragma once
#include <dctl/utility/constexpr.hpp>

namespace dctl {
namespace angle {

template<class T>
constexpr auto make_angle(T const& n) noexcept
{
        return util::abs_modulus(n, 360);
}

template<class T>
constexpr auto is_angle(T const& n) noexcept
{
        return util::is_abs_modulus(n, 360);
}

template<class T>
constexpr auto inverse(T const& alpha) noexcept
{
        return angle::make_angle(-alpha);
}

template<class T>
constexpr auto rotate(T const& alpha, T const& theta) noexcept
{
        return angle::make_angle(alpha + theta);
}

template<class T>
constexpr auto mirror(T const& alpha, T const& theta) noexcept
{
        return angle::rotate(angle::inverse(angle::rotate(alpha, angle::inverse(theta))), theta);
}

template<int N>
struct Degrees
{
        static constexpr auto value = make_angle(N); // N is of non-class type, so no ADL for make_angle
        static_assert(0 <= value && value < 360, "Angles have to lie in the range [0, 360)");

        using type = Degrees<value>;
};

/*

        Degrees are denoted as <D><XXX>, where XXX is exactly three characters
        long, running from 000 until 360 in steps of 45. Counterclockwise
        rotations are denoted as <L><XXX>, where <XXX> runs from 0 until 180.
        Clockwise rotations are similarly denoted as <R><XXX>.

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

// named multiples of 45 degrees

using D000 = Degrees<  0>;
using D045 = Degrees< 45>;
using D090 = Degrees< 90>;
using D135 = Degrees<135>;
using D180 = Degrees<180>;
using D225 = Degrees<225>;
using D270 = Degrees<270>;
using D315 = Degrees<315>;
using D360 = Degrees<360>;

// counterclockwise (left)

using L000 = D000;
using L045 = D045;
using L090 = D090;
using L135 = D135;
using L180 = D180;

// clockwise (right)

using R180 = D180;
using R135 = D225;
using R090 = D270;
using R045 = D315;
using R000 = D360;

}       // namespace angle
}       // namespace dctl
