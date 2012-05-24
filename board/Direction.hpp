#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Angle.hpp"
#include "Degrees.hpp"
#include "Transform.hpp"

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
        typename Board
>
struct Direction
{
private:
        BOOST_STATIC_CONSTANT(auto, N = (Color? degrees::D000 : degrees::D180) + Board::inverse_angle::value );
        typedef angle< N > A;

public:
        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        typedef typename rotate< angle<degrees::D000>, A >::type right;
        typedef typename rotate< angle<degrees::D045>, A >::type right_up;
        typedef typename rotate< angle<degrees::D090>, A >::type up;
        typedef typename rotate< angle<degrees::D135>, A >::type left_up;
        typedef typename rotate< angle<degrees::D180>, A >::type left;
        typedef typename rotate< angle<degrees::D225>, A >::type left_down;
        typedef typename rotate< angle<degrees::D270>, A >::type down;
        typedef typename rotate< angle<degrees::D315>, A >::type right_down;
};

}       // namespace board
}       // namespace dctl
