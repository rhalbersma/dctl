#pragma once
#include <boost/mpl/bool.hpp>           // bool_
#include <boost/mpl/eval_if.hpp>        // eval_if
#include "Degrees.hpp"
#include "Transform.hpp"

namespace dctl {
namespace angle {

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
        typename Board
>
struct Compass
{
private:
        typedef typename rotate<
                typename Board::inverse_angle,
                boost::mpl::eval_if<
                        boost::mpl::bool_<Color>,
                        D000,
                        D180
                >
        >::type A;

public:
        typedef typename rotate< D000, A >::type right;
        typedef typename rotate< D045, A >::type right_up;
        typedef typename rotate< D090, A >::type up;
        typedef typename rotate< D135, A >::type left_up;
        typedef typename rotate< D180, A >::type left;
        typedef typename rotate< D225, A >::type left_down;
        typedef typename rotate< D270, A >::type down;
        typedef typename rotate< D315, A >::type right_down;
};

}       // namespace angle
}       // namespace dctl
