#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/int.hpp>            // boost::mpl::int_
#include <boost/mpl/integral_c_tag.hpp> // boost::mpl::integral_c_tag
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include "Degrees.hpp"
#include "Modular.hpp"
#include "Transform.hpp"
#include <boost/mpl/identity.hpp>

namespace dctl {

template<int N>
struct angle
{
        BOOST_STATIC_CONSTANT(auto, value = mod_360< boost::mpl::int_<N> >::value);

        // lazily evaluable metadata == nullary metafunction
        typedef angle<N> type;
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
