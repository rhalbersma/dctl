#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Angle.hpp"
#include "Degrees.hpp"
#include "Modular.hpp"
#include "Transform.hpp"

namespace dctl {
namespace board {

template
<
        int D = 2,                              // "demilitarized" rows in the initial position
        int G = 2,                              // number of ghost bit columns
        typename A = angle<degrees::D000>       // rotation from external to internal grid
>
struct Structure
{
        // reflection on template parameters
        BOOST_STATIC_CONSTANT(auto, dmz = D);
        BOOST_STATIC_CONSTANT(auto, ghosts = G);
        typedef A full_angle;
        typedef typename inverse< full_angle >::type inverse_angle;
};

}       // namespace board
}       // namespace dctl
