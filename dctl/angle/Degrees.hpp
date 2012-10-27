#pragma once
#include <boost/mpl/int.hpp>            // int_
#include <dctl/mpl/modular.hpp>         // abs_modulus

namespace dctl {
namespace angle {

template<int N>
struct Degrees
{
        // nullary metadata subject to arithmetic modulo 360 degrees
        enum { value = mpl::abs_modulus<
        	        boost::mpl::int_<N>,
        		boost::mpl::int_<360>
                >::value
        };

        // lazily evaluable metadata == nullary metafunction
        typedef Degrees<value> type;
};

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

typedef Degrees<  0> D000;
typedef Degrees< 45> D045;
typedef Degrees< 90> D090;
typedef Degrees<135> D135;
typedef Degrees<180> D180;
typedef Degrees<225> D225;
typedef Degrees<270> D270;
typedef Degrees<315> D315;
typedef Degrees<360> D360;

typedef D045 L045;      // counterclockwise
typedef D090 L090;      // counterclockwise
typedef D135 L135;      // counterclockwise
typedef D225 R135;      // clockwise
typedef D270 R090;      // clockwise
typedef D315 R045;      // clockwise

}       // namespace angle
}       // namespace dctl
