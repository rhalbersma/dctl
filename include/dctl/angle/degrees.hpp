#pragma once

namespace dctl {
namespace angle {
namespace detail {

template<class T>
constexpr T abs(T const& t)
{
        return (t < 0)? -t : t;
}

template<class T>
constexpr T abs_modulus(T const& n, T const& d)
{
        return n % d + ((n % d < 0)? detail::abs(d) : 0);
}

}       // detail

template<int N>
struct Degrees
{
        static constexpr auto value = detail::abs_modulus(N, 360);
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

using D000 = Degrees<  0>;
using D045 = Degrees< 45>;
using D090 = Degrees< 90>;
using D135 = Degrees<135>;
using D180 = Degrees<180>;
using D225 = Degrees<225>;
using D270 = Degrees<270>;
using D315 = Degrees<315>;
using D360 = Degrees<360>;

// counterclockwise

using L000 = D000;
using L045 = D045;
using L090 = D090;
using L135 = D135;
using L180 = D180;

// clockwise

using R180 = D180;
using R135 = D225;
using R090 = D270;
using R045 = D315;
using R000 = D360;

}       // namespace angle
}       // namespace dctl
