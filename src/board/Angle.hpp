#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/int.hpp>            // int_
#include "Modular.hpp"                  // abs_modulus
#include "Transform.hpp"                // inverse, rotate, mirror

namespace dctl {

template<int N>
struct Angle
{
        // nullary metadata subject to arithmetic modulo 360 degrees
        static int const value = boost::mpl::modulus<boost::mpl::int_<N+8>, boost::mpl::int_<8> >::value; //Mod<N, 8>::value;

        // lazily evaluable metadata == nullary metafunction
        typedef Angle<value> type;
};

template<int N>
struct inverse< Angle<N> >
:
        Angle< -N >
{};

template<int N1, int N2>
struct rotate< Angle<N1>, Angle<N2> >
:
        Angle< N1 + N2 >
{};

template<int N1, int N2>
struct mirror< Angle<N1>, Angle<N2> >
:
        rotate<
                inverse<
                        rotate<
                                Angle<N1>,
                                inverse< Angle<N2> >
                        >
                >,
                Angle<N2>
        >
{};

}       // namespace dctl
