#pragma once
#include <boost/mpl/bool.hpp>           // bool_
#include <boost/mpl/eval_if.hpp>        // eval_if
#include "../angle/Degrees.hpp"
#include "../mpl/transform.hpp"

namespace dctl {

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
        typedef typename mpl::rotate< typename
        	Board::inverse_angle,
                boost::mpl::eval_if<
                        boost::mpl::bool_<Color>,
                        angle::D000,
                        angle::D180
                >
        >::type A;

public:
        typedef typename mpl::rotate< angle::D000, A >::type right;
        typedef typename mpl::rotate< angle::D045, A >::type right_up;
        typedef typename mpl::rotate< angle::D090, A >::type up;
        typedef typename mpl::rotate< angle::D135, A >::type left_up;
        typedef typename mpl::rotate< angle::D180, A >::type left;
        typedef typename mpl::rotate< angle::D225, A >::type left_down;
        typedef typename mpl::rotate< angle::D270, A >::type down;
        typedef typename mpl::rotate< angle::D315, A >::type right_down;
};

}       // namespace dctl
