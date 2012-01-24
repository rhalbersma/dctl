#pragma once
#include <boost/mpl/arithmetic.hpp>     // boost::mpl:: divides, modulus
#include <boost/mpl/comparison.hpp>     // boost::mpl:: equal_to, not_equal_to
#include <boost/mpl/int.hpp>            // boost::mpl:: int_
#include <boost/mpl/logical.hpp>        // boost::mpl:: not_, and_, or_
#include "Angle.hpp"
#include "Degrees.hpp"
#include "Traits_fwd.hpp"
#include "Transform.hpp"

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
struct is_orthogonal< angle<N> >
: 
        // right, up, left, down
        boost::mpl::equal_to<
                boost::mpl::modulus< 
                        boost::mpl::int_<N>, 
                        boost::mpl::int_<degrees::D090> 
                >,
                boost::mpl::int_<0>
        >
{}; 

template<int N> 
struct is_diagonal< angle<N> >      
:
        // right_up, left_up, left_down, right_down
        boost::mpl::not_< is_orthogonal< angle<N> > > 
{};

template<int N> 
struct is_up< angle<N> >            
:
        // right_up, up, left_up
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

template<int N> 
struct is_down< angle<N> >
:
        // left_down, down, right_down
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

template<int N> 
struct is_left< angle<N> >
:
        // left_up, left, left_down
        is_down< rotate< angle<N>, angle<degrees::L090> > >
{};   

template<int N> 
struct is_right< angle<N> >          
:
        // right, right_up, right_down
        is_up< rotate< angle<N>, angle<degrees::L090> > >
{};

template<int N> 
struct is_positive< angle<N> >
:
        // right_up, up, left_up, left
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

template<int N> 
struct is_negative< angle<N> >      
: 
        // right, left_down, down, right_down
        boost::mpl::not_< is_positive< angle<N> > > 
{};  

}       // namespace board
}       // namespace dctl
