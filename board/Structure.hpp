#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Angle.hpp"
#include "Degrees.hpp"
#include "Transform.hpp"

namespace dctl {
namespace board {

template
<
        int D = 2,              // "demilitarized" rows in the initial position
        int G = 2,              // number of ghost bit columns
        int A = Degrees::D000   // rotation from external to internal grid
>
struct Structure
{
        BOOST_STATIC_ASSERT(D > 0 && G > 0 && !mod_090<A>::value);

        // reflection on template parameters
        BOOST_STATIC_CONSTANT(auto, dmz = D);
        BOOST_STATIC_CONSTANT(auto, ghosts = G);
        BOOST_STATIC_CONSTANT(auto, angle = mod_360<A>::value);
        BOOST_STATIC_CONSTANT(auto, inverse_angle = inverse<angle>::type::value);
};

}       // namespace board
}       // namespace dctl
