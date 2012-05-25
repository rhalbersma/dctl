#pragma once
#include "Angle.hpp"

namespace dctl {
namespace angle {

/*

        Degrees are denoted as <D><XXX>, where XXX is exactly three characters
        long, running from 000 until 360 in steps of 45. Counterclockwise
        rotations are denoted as <L><XXX>, where <XXX> runs from 45 until 135.
        Clockwise rotations are similarly denoted as <R><XXX>.

                D090 = L090
                     |
        D135 = L135  |  D045 = L045
                   \ | /
                    \|/
           D180 ----- ----- D000 = D360
                    /|\
                   / | \
        D225 = R135  |  D315 = R045
                     |
                D270 = R090

*/

typedef Angle<0> D000;
typedef Angle<1> D045;
typedef Angle<2> D090;
typedef Angle<3> D135;
typedef Angle<4> D180;
typedef Angle<5> D225;
typedef Angle<6> D270;
typedef Angle<7> D315;
typedef Angle<8> D360;

typedef D045 L045;      // counterclockwise
typedef D090 L090;      // counterclockwise
typedef D135 L135;      // counterclockwise
typedef D225 R135;      // clockwise
typedef D270 R090;      // clockwise
typedef D315 R045;      // clockwise

}       // namespace angle
}       // namespace dctl
