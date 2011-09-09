#pragma once
#include <boost/config.hpp>
#include "Angle.h"
#include "../utils/TemplateTricks.h"

namespace dctl {
namespace board {

// direction indices are subject to arithmetic modulo 8, with the unit element equal to 1
/*                      
             2
          3  |  1
           \ | / 
            \|/
        4---- ----0
            /|\
           / | \
          5  |  7
             6                     
*/

// the 8 diagonal and orthogonal direction indices for black and white to move
template<bool Color, typename Board> 
class Direction
{ 
private:                        
        BOOST_STATIC_CONSTANT(auto, A = (Angle::D180 * !Color) + Board::inverse_angle);

public:
        // NOTE: parenthesize rotate<..., ...> to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, right      = (rotate<Int2Type<Angle::D000>, A>::value));
        BOOST_STATIC_CONSTANT(auto, right_up   = (rotate<Int2Type<Angle::D045>, A>::value));
        BOOST_STATIC_CONSTANT(auto, up         = (rotate<Int2Type<Angle::D090>, A>::value));
        BOOST_STATIC_CONSTANT(auto, left_up    = (rotate<Int2Type<Angle::D135>, A>::value));
        BOOST_STATIC_CONSTANT(auto, left       = (rotate<Int2Type<Angle::D180>, A>::value));
        BOOST_STATIC_CONSTANT(auto, left_down  = (rotate<Int2Type<Angle::D225>, A>::value));
        BOOST_STATIC_CONSTANT(auto, down       = (rotate<Int2Type<Angle::D270>, A>::value));
        BOOST_STATIC_CONSTANT(auto, right_down = (rotate<Int2Type<Angle::D315>, A>::value));
};

}       // namespace board
}       // namespace dctl
