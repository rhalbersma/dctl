#pragma once
#include <boost/mpl/int.hpp>            // int_
#include <dctl/angle/degrees.hpp>       // D000
#include <dctl/angle/transform.hpp>     // inverse

namespace dctl {
namespace board {

template
<
        int Columns = 2,
        typename Angle = angle::D000    // rotation from external to internal grid
>
struct Ghosts
{
        // reflection on template parameters
        typedef boost::mpl::int_<Columns> columns;
        typedef Angle full_angle;
        typedef typename mpl::lazy::inverse< full_angle >::type inverse_angle;
};

}       // namespace board
}       // namespace dctl
