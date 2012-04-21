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
        BOOST_STATIC_CONSTANT(auto, right      = (rotate< angle<degrees::D000>, A >::value));
        BOOST_STATIC_CONSTANT(auto, right_up   = (rotate< angle<degrees::D045>, A >::value));
        BOOST_STATIC_CONSTANT(auto, up         = (rotate< angle<degrees::D090>, A >::value));
        BOOST_STATIC_CONSTANT(auto, left_up    = (rotate< angle<degrees::D135>, A >::value));
        BOOST_STATIC_CONSTANT(auto, left       = (rotate< angle<degrees::D180>, A >::value));
        BOOST_STATIC_CONSTANT(auto, left_down  = (rotate< angle<degrees::D225>, A >::value));
        BOOST_STATIC_CONSTANT(auto, down       = (rotate< angle<degrees::D270>, A >::value));
        BOOST_STATIC_CONSTANT(auto, right_down = (rotate< angle<degrees::D315>, A >::value));
};

}       // namespace board
}       // namespace dctl
