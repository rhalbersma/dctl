#pragma once
#include <boost/mpl/int.hpp>            // int_
#include <dctl/angle/Degrees.hpp>       // D000
#include <dctl/angle/transform.hpp>     // inverse

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

        typedef typename mpl::lazy::inverse< full_angle >::type inverse_angle;
};

}       // namespace board
}       // namespace dctl
