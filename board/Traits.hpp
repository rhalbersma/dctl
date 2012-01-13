#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include "Degrees.hpp"
#include "Transform.hpp"
#include "../utility/Int2Type.hpp"

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

template<int N> 
struct is_diagonal      
{
        BOOST_STATIC_ASSERT(Degrees::D000 <= N && N < Degrees::D360);

        // right_up, left_up, left_down, right_down
        BOOST_STATIC_CONSTANT(auto, value = (N % 2) != 0); 
};

template<int N> 
struct is_orthogonal    
{
        BOOST_STATIC_ASSERT(Degrees::D000 <= N && N < Degrees::D360);

        // right, up, left, down
        BOOST_STATIC_CONSTANT(auto, value = !is_diagonal<N>::value); 
};

template<int N> 
struct is_up            
{ 
        BOOST_STATIC_ASSERT(Degrees::D000 <= N && N < Degrees::D360);

        // right_up, up, left_up
        BOOST_STATIC_CONSTANT(auto, value = !(N / 4) && (N % 4)); 
};

template<int N> 
struct is_down          
{ 
        BOOST_STATIC_ASSERT(Degrees::D000 <= N && N < Degrees::D360);

        // left_down, down, right_down
        BOOST_STATIC_CONSTANT(auto, value = (N / 4) && (N % 4)); 
};

template<int N> 
struct is_left         
{ 
        BOOST_STATIC_ASSERT(Degrees::D000 <= N && N < Degrees::D360);

        // left_up, left, left_down
        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (is_down<rotate<Angle<N>, Degrees::L090>::type::value>::value)
        );
};   

template<int N> 
struct is_right          
{ 
        BOOST_STATIC_ASSERT(Degrees::D000 <= N && N < Degrees::D360);

        // right, right_up, right_down
        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (is_up<rotate<Angle<N>, Degrees::L090>::type::value>::value)
        );
};     

template<int N> 
struct is_positive      
{ 
        BOOST_STATIC_ASSERT(Degrees::D000 <= N && N < Degrees::D360);

        // right_up, up, left_up, left
        BOOST_STATIC_CONSTANT(auto, value = 
                is_up<N>::value || 
                (is_left<N>::value && !is_down<N>::value)
        ); 
};

template<int N> 
struct is_negative      
{ 
        BOOST_STATIC_ASSERT(Degrees::D000 <= N && N < Degrees::D360);

        // right, left_down, down, right_down
        BOOST_STATIC_CONSTANT(auto, value = !is_positive<N>::value); 
};  

}       // namespace board
}       // namespace dctl
