#pragma once
#include <boost/config.hpp>
#include "Angle.h"
#include "Transform.h"
#include "../utility/TemplateTricks.h"

namespace dctl {
namespace board {

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

template<int Index> 
struct is_diagonal      
{
        // right_up, left_up, left_down, right_down
        BOOST_STATIC_CONSTANT(auto, value = (Index % 2) != 0); 
};

template<int Index> 
struct is_orthogonal    
{ 
        // right, up, left, down
        BOOST_STATIC_CONSTANT(auto, value = !is_diagonal<Index>::value); 
};

template<int Index> 
struct is_up            
{ 
        // right_up, up, left_up
        BOOST_STATIC_CONSTANT(auto, value = !(Index / 4) && (Index % 4)); 
};

template<int Index> 
struct is_down          
{ 
        // left_down, down, right_down
        BOOST_STATIC_CONSTANT(auto, value = (Index / 4) && (Index % 4)); 
};

template<int Index> 
struct is_left         
{ 
        // left_up, left, left_down
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (is_down<rotate<Angle<Index>, Degrees::L090>::type::index>::value)
        );
};   

template<int Index> 
struct is_right          
{ 
        // right, right_up, right_down
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (is_up<rotate<Angle<Index>, Degrees::L090>::type::index>::value)
        );
};     

template<int Index> 
struct is_positive      
{ 
        // right_up, up, left_up, left
        BOOST_STATIC_CONSTANT(auto, value = 
                is_up<Index>::value || 
                (is_left<Index>::value && !is_down<Index>::value)
        ); 
};

template<int Index> 
struct is_negative      
{ 
        // right, left_down, down, right_down
        BOOST_STATIC_CONSTANT(auto, value = !is_positive<Index>::value); 
};  

}       // namespace board
}       // namespace dctl
