#pragma once
#include <boost/mpl/int_fwd.hpp>        // int_
#include "Angle.hpp"
#include "Degrees.hpp"
#include "Modular.hpp"
#include "Transform.hpp"

namespace dctl {
namespace board {

template
<
        int D = 2,                      // "demilitarized" rows in the initial position
        int G = 2,                      // number of ghost bit columns
        typename A = angle::D000        // rotation from external to internal grid
>
struct Structure
{
        // reflection on template parameters
        typedef boost::mpl::int_<D> dmz;
        typedef boost::mpl::int_<G> ghosts;
        typedef A full_angle;

        typedef typename inverse< full_angle >::type inverse_angle;
};

}       // namespace board
}       // namespace dctl