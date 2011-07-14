#pragma once
#include "Angle.h"
#include "Transform.h"
#include "../Utilities/TemplateTricks.h"

namespace board {

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

// diagonal, orthogonal
template<int I> struct is_diagonal      { enum { value = I % 2                    }; }; // 1, 3, 5, 7
template<int I> struct is_orthogonal    { enum { value = !is_diagonal<I>::value   }; }; // 0, 2, 4, 6

// up, down, right, left
template<int I> struct is_up            { enum { value = !(I / 4) && (I % 4)      }; }; // 1, 2, 3
template<int I> struct is_down          { enum { value =  (I / 4) && (I % 4)      }; }; // 5, 6, 7

template<int I> 
struct is_right          
{ 
        enum { value = is_up<Rotate<Int2Type<I>, Angle::L090>::value>::value };         // 0, 1, 7
};     

template<int I> 
struct is_left         
{ 
        enum { value = is_down<Rotate<Int2Type<I>, Angle::L090>::value>::value };       // 3, 4, 5
};   

// negative, positive
template<int I> struct is_negative      { enum { value = !is_positive<I>::value  }; };  // 5, 6, 7, 0 

template<int I> 
struct is_positive      
{ 
        enum { value = is_up<I>::value || (is_left<I>::value && !is_down<I>::value) };  // 1, 2, 3, 4 
};

}       // namespace board
