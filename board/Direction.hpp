#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Angle.hpp"
#include "Transform.hpp"
#include "../utility/Int2Type.hpp"

namespace dctl {
namespace board {

// indices for the 8 named directions from the black point of view
/*                

        +++ WHITE POINT OF VIEW +++

                  up = 6
                     |
        7 = left_up  |  right_up = 5
                   \ | / 
                    \|/
       0 = left ----- ----- right = 4
                    /|\
                   / | \
      1 = left_down  |  right_down = 3
                     |
                down = 2

        +++ BLACK POINT OF VIEW +++
                
*/

// indices for the 8 named directions from the white point of view
/*                      

        +++ BLACK POINT OF VIEW +++

                  up = 2
                     |
        3 = left_up  |  right_up = 1
                   \ | / 
                    \|/
       4 = left ----- ----- right = 0
                    /|\
                   / | \
      5 = left_down  |  right_down = 7
                     |
                down = 6

        +++ WHITE POINT OF VIEW +++
                
*/

// indices for the 8 named directions from the black and white point of view
template<
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
        BOOST_STATIC_CONSTANT(auto, right      = (rotate<angle<degrees::D000>, A>::type::value));
        BOOST_STATIC_CONSTANT(auto, right_up   = (rotate<angle<degrees::D045>, A>::type::value));
        BOOST_STATIC_CONSTANT(auto, up         = (rotate<angle<degrees::D090>, A>::type::value));
        BOOST_STATIC_CONSTANT(auto, left_up    = (rotate<angle<degrees::D135>, A>::type::value));
        BOOST_STATIC_CONSTANT(auto, left       = (rotate<angle<degrees::D180>, A>::type::value));
        BOOST_STATIC_CONSTANT(auto, left_down  = (rotate<angle<degrees::D225>, A>::type::value));
        BOOST_STATIC_CONSTANT(auto, down       = (rotate<angle<degrees::D270>, A>::type::value));
        BOOST_STATIC_CONSTANT(auto, right_down = (rotate<angle<degrees::D315>, A>::type::value));
};

}       // namespace board
}       // namespace dctl
