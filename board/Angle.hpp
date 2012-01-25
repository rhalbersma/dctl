#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/int.hpp>            // int_
#include "Degrees.hpp"
#include "Modular.hpp"
#include "Transform.hpp"

namespace dctl {

template<int N>
struct angle_tag
:
        boost::mpl::int_<N>
{};

template<int N>
struct angle
{
        // tag-dispatching for integral constants
        typedef typename boost::mpl::int_<N>::tag tag;

        // angles are subject to arithmetic modulo 360 degrees
        BOOST_STATIC_CONSTANT(auto, value = 
        (        
                abs_modulus< 
                        boost::mpl::int_<N>, 
                        boost::mpl::int_<degrees::D360> 
                >::value
        ));
        
        // lazily evaluable metadata == nullary metafunction
        typedef angle<value> type;      
};

template<int N1, int N2>
struct rotate< angle<N1>, angle<N2> >
:
        angle<N1 + N2> 
{};

template<int N>
struct inverse< angle<N> >
:
        angle<-N>
{};

template<int N>
struct mirror_up< angle<N> >
:       
        angle<
                (angle<N>::value + degrees::D090) % degrees::D180 + 
                (angle<N>::value / degrees::D180) * degrees::D180
        >
{};

template<int N>
struct mirror_down< angle<N> >
:
        rotate<
                mirror_up<
                        rotate<
                                angle<N>, 
                                angle<degrees::L090>
                        >
                >,
                angle<degrees::R090>
        >
{};

}       // namespace dctl
