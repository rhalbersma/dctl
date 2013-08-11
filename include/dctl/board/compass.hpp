#pragma once
#include <boost/mpl/bool.hpp>           // bool_
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <dctl/angle/degrees.hpp>       // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/transform.hpp>     // rotate

namespace dctl {
namespace board {

/*

     +++ PASSIVE COLOR POINT OF VIEW +++

                    up
                     |
            left_up  |  right_up
                   \ | /
                    \|/
           left ----- ----- right
                    /|\
                   / | \
          left_down  |  right_down
                     |
                   down

      +++ ACTIVE COLOR POINT OF VIEW +++

*/

template
<
        bool Color,
        class Board
>
struct Compass
{
private:
        using A = mpl::lazy::rotate<
        	mpl::lazy::inverse< angle::Degrees< Board::orientation > >,
                boost::mpl::eval_if<
                        boost::mpl::bool_<Color>,
                        angle::D000,
                        angle::D180
                >
        >;

public:
        using right      = typename mpl::lazy::rotate< angle::D000, A >::type;
        using right_up   = typename mpl::lazy::rotate< angle::D045, A >::type;
        using up         = typename mpl::lazy::rotate< angle::D090, A >::type;
        using left_up    = typename mpl::lazy::rotate< angle::D135, A >::type;
        using left       = typename mpl::lazy::rotate< angle::D180, A >::type;
        using left_down  = typename mpl::lazy::rotate< angle::D225, A >::type;
        using down       = typename mpl::lazy::rotate< angle::D270, A >::type;
        using right_down = typename mpl::lazy::rotate< angle::D315, A >::type;
};

}       // namespace board
}       // namespace dctl
