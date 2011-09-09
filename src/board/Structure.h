#pragma once
#include <boost/config.hpp>
#include "Angle.h"
#include "Transform.h"

namespace dctl {
namespace board {

template
<
        int D = 2,              // "demilitarized" rows in the initial position
        int G = 2,              // number of ghost bit columns
        int A = Angle::D000     // rotation from external to internal grid
>
struct Structure
{
        // reflection on template parameters
        BOOST_STATIC_CONSTANT(auto, dmz = D);
        BOOST_STATIC_CONSTANT(auto, ghosts = G);
        BOOST_STATIC_CONSTANT(auto, angle = A);
        BOOST_STATIC_CONSTANT(auto, inverse_angle = inverse<A>::value);
};

}       // namespace board
}       // namespace dctl
