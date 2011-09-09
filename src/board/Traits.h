#pragma once
#include <boost/config.hpp>
#include "Angle.h"
#include "Transform.h"
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

template<int I> 
struct is_diagonal      
{
        // 1, 3, 5, 7
        BOOST_STATIC_CONSTANT(auto, value = (I % 2) != 0); 
};

template<int I> 
struct is_orthogonal    
{ 
        // 0, 2, 4, 6
        BOOST_STATIC_CONSTANT(auto, value = !is_diagonal<I>::value); 
};

template<int I> 
struct is_up            
{ 
        // 1, 2, 3
        BOOST_STATIC_CONSTANT(auto, value = !(I / 4) && (I % 4)); 
};

template<int I> 
struct is_down          
{ 
        // 5, 6, 7
        BOOST_STATIC_CONSTANT(auto, value = (I / 4) && (I % 4)); 
};

template<int I> 
struct is_left         
{ 
        // 3, 4, 5
        // NOTE: parenthesize rotate<..., ...> to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (is_down<rotate<Int2Type<I>, Angle::L090>::value>::value)
        );
};   

template<int I> 
struct is_right          
{ 
        // 0, 1, 7
        // NOTE: parenthesize rotate<..., ...> to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (is_up<rotate<Int2Type<I>, Angle::L090>::value>::value)
        );
};     

template<int I> 
struct is_positive      
{ 
        // 1, 2, 3, 4
        BOOST_STATIC_CONSTANT(auto, value = 
                is_up<I>::value || 
                (is_left<I>::value && !is_down<I>::value)
        ); 
};

template<int I> 
struct is_negative      
{ 
        // 5, 6, 7, 0
        BOOST_STATIC_CONSTANT(auto, value = !is_positive<I>::value); 
};  

}       // namespace board
}       // namespace dctl
