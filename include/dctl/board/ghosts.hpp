#pragma once
#include <boost/mpl/int.hpp>            // int_
#include <dctl/angle/degrees.hpp>       // D000
#include <dctl/angle/transform.hpp>     // inverse

namespace dctl {
namespace board {

template
<
        int Columns = 2,
        class Angle = angle::D000    // rotation from external to internal grid
>
struct Ghosts
{
        // reflection on template parameters
        using columns = boost::mpl::int_<Columns>;
        using full_angle = Angle;
        using inverse_angle = typename mpl::lazy::inverse< full_angle >::type;
};

}       // namespace board
}       // namespace dctl
