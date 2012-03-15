#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/int.hpp>            // int_
#include "Degrees.hpp"                  // D360
#include "Modular.hpp"                  // abs_modulus
#include "Transform.hpp"                // inverse, rotate, mirror

namespace dctl {

template<int N>
struct angle
{
        // nullary metadata subject to arithmetic modulo 360 degrees
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

template<int N>
struct inverse< angle<N> >
:
        angle< -N >
{};

template<int N1, int N2>
struct rotate< angle<N1>, angle<N2> >
:
        angle< N1 + N2 >
{};

template<int N1, int N2>
struct mirror< angle<N1>, angle<N2> >
:
        rotate<
                inverse<
                        rotate<
                                angle<N1>,
                                inverse< angle<N2> >
                        >
                >,
                angle<N2>
        >
{};

}       // namespace dctl
