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

static constexpr auto D000 =   0;
static constexpr auto D045 =  45;
static constexpr auto D090 =  90;
static constexpr auto D135 = 135;
static constexpr auto D180 = 180;
static constexpr auto D225 = 225;
static constexpr auto D270 = 270;
static constexpr auto D315 = 315;
static constexpr auto D360 = 360;

// counterclockwise (left)

static constexpr auto L000 = D000;
static constexpr auto L045 = D045;
static constexpr auto L090 = D090;
static constexpr auto L135 = D135;
static constexpr auto L180 = D180;

// clockwise (right)

static constexpr auto R180 = D180;
static constexpr auto R135 = D225;
static constexpr auto R090 = D270;
static constexpr auto R045 = D315;
static constexpr auto R000 = D360;

}       // namespace angle
}       // namespace dctl
