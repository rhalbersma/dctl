#pragma once
#include <boost/config.hpp>
#include "Angle.h"
#include "Transform.h"
#include "../utility/TemplateTricks.h"

namespace dctl {
namespace board {

// indices for the 8 named directions from the black point of view
/*                      
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
*/

// indices for the 8 named directions from the white point of view
/*                      
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
*/

// indices for the 8 named directions from the black and white point of view
template<bool Color, typename Board> 
class Direction
{ 
private:                        
        BOOST_STATIC_CONSTANT(auto, A = (Color? Degrees::D000 : Degrees::D180) + Board::inverse_angle);

public:
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, right      = (rotate<Angle<Degrees::D000>, A>::type::index));
        BOOST_STATIC_CONSTANT(auto, right_up   = (rotate<Angle<Degrees::D045>, A>::type::index));
        BOOST_STATIC_CONSTANT(auto, up         = (rotate<Angle<Degrees::D090>, A>::type::index));
        BOOST_STATIC_CONSTANT(auto, left_up    = (rotate<Angle<Degrees::D135>, A>::type::index));
        BOOST_STATIC_CONSTANT(auto, left       = (rotate<Angle<Degrees::D180>, A>::type::index));
        BOOST_STATIC_CONSTANT(auto, left_down  = (rotate<Angle<Degrees::D225>, A>::type::index));
        BOOST_STATIC_CONSTANT(auto, down       = (rotate<Angle<Degrees::D270>, A>::type::index));
        BOOST_STATIC_CONSTANT(auto, right_down = (rotate<Angle<Degrees::D315>, A>::type::index));
};

}       // namespace board
}       // namespace dctl
