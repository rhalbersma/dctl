#pragma once
#include <boost/mpl/arithmetic.hpp>     // divides, modulus
#include <boost/mpl/comparison.hpp>     // equal_to, not_equal_to
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // not_, and_, or_
#include "Traits_fwd.hpp"
#include "Angle.hpp"                    // angle, rotate
#include "Degrees.hpp"                  // L090, D090, D180 
#include "Transform.hpp"                // rotate

namespace dctl {
namespace board {

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

// right, up, left, down
template<int N> 
struct is_orthogonal< angle<N> >
: 
        boost::mpl::equal_to<
                boost::mpl::modulus< 
                        boost::mpl::int_<N>, 
                        boost::mpl::int_<degrees::D090> 
                >,
                boost::mpl::int_<0>
        >
{}; 

// right_up, left_up, left_down, right_down
template<int N> 
struct is_diagonal< angle<N> >      
:
        boost::mpl::not_< is_orthogonal< angle<N> > > 
{};

// right_up, up, left_up
template<int N> 
struct is_up< angle<N> >            
:
        boost::mpl::and_<                 
                boost::mpl::equal_to<
                        boost::mpl::divides< 
                                boost::mpl::int_<N>, 
                                boost::mpl::int_<degrees::D180>
                        >,
                        boost::mpl::int_<0>
                >,
                boost::mpl::not_equal_to<
                        boost::mpl::modulus<
                                boost::mpl::int_<N>, 
                                boost::mpl::int_<degrees::D180>                        
                        >,
                        boost::mpl::int_<0>
                >
        >
{};

// left_down, down, right_down
template<int N> 
struct is_down< angle<N> >
:
        boost::mpl::and_<                 
                boost::mpl::not_equal_to<
                        boost::mpl::divides< 
                                boost::mpl::int_<N>, 
                                boost::mpl::int_<degrees::D180>
                        >,
                        boost::mpl::int_<0>
                >,
                boost::mpl::not_equal_to<
                        boost::mpl::modulus<
                                boost::mpl::int_<N>, 
                                boost::mpl::int_<degrees::D180>                        
                        >,
                        boost::mpl::int_<0>
                >
        >
{};

// left_up, left, left_down
template<int N> 
struct is_left< angle<N> >
:
        is_down< rotate< angle<N>, angle<degrees::L090> > >
{};   

// right, right_up, right_down
template<int N> 
struct is_right< angle<N> >          
:
        is_up< rotate< angle<N>, angle<degrees::L090> > >
{};

// right_up, up, left_up, left
template<int N> 
struct is_positive< angle<N> >
:
        boost::mpl::or_<
                is_up< angle<N> >,
                boost::mpl::and_<
                        is_left< angle<N> >,
                        boost::mpl::not_< 
                                is_down< angle<N> >                         
                        >
                >
        >
{};

// right, left_down, down, right_down
template<int N> 
struct is_negative< angle<N> >      
: 
        boost::mpl::not_< is_positive< angle<N> > > 
{};  

}       // namespace board
}       // namespace dctl
