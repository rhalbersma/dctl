#pragma once
#include <boost/mpl/arithmetic.hpp>     // divides, modulus
#include <boost/mpl/comparison.hpp>     // equal_to, not_equal_to
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // not_, and_, or_
#include "Traits_fwd.hpp"
#include "Degrees.hpp"                  // L090, D090, D180
#include "../mpl/transform.hpp"		// rotate

namespace dctl {
namespace angle {

/*

         <-- LEAST SIGNIFICANT BITS

                D090 = L090
                     |
        D135 = L135  |  D045 = L045
                   \ | /
                    \|/
           D180 ----- ----- D000 = D360
                    /|\
                   / | \
        D225 = R135  |  D315 = R045
                     |
                D270 = R090

          MOST SIGNIFICANT BITS -->

*/

template<int N>
struct is_orthogonal< Degrees<N> >
:
        boost::mpl::equal_to<
                boost::mpl::modulus<
                        boost::mpl::int_<N>,
                        boost::mpl::int_<D090::value>
                >,
                boost::mpl::int_<0>
        >
{};

template<int N>
struct is_diagonal< Degrees<N> >
:
        boost::mpl::not_< is_orthogonal< Degrees<N> > >
{};

template<int N>
struct is_up< Degrees<N> >
:
        boost::mpl::and_<
                boost::mpl::equal_to<
                        boost::mpl::divides<
                                boost::mpl::int_<N>,
                                boost::mpl::int_<D180::value>
                        >,
                        boost::mpl::int_<0>
                >,
                boost::mpl::not_equal_to<
                        boost::mpl::modulus<
                                boost::mpl::int_<N>,
                                boost::mpl::int_<D180::value>
                        >,
                        boost::mpl::int_<0>
                >
        >
{};

template<int N>
struct is_down< Degrees<N> >
:
        boost::mpl::and_<
                boost::mpl::not_equal_to<
                        boost::mpl::divides<
                                boost::mpl::int_<N>,
                                boost::mpl::int_<D180::value>
                        >,
                        boost::mpl::int_<0>
                >,
                boost::mpl::not_equal_to<
                        boost::mpl::modulus<
                                boost::mpl::int_<N>,
                                boost::mpl::int_<D180::value>
                        >,
                        boost::mpl::int_<0>
                >
        >
{};

template<int N>
struct is_left< Degrees<N> >
:
        is_down< mpl::rotate< Degrees<N>, L090 > >
{};

template<int N>
struct is_right< Degrees<N> >
:
        is_up< mpl::rotate< Degrees<N>, L090 > >
{};

template<int N>
struct is_positive< Degrees<N> >
:
        boost::mpl::or_<
                is_up< Degrees<N> >,
                boost::mpl::and_<
                        is_left< Degrees<N> >,
                        boost::mpl::not_<
                                is_down< Degrees<N> >
                        >
                >
        >
{};

template<int N>
struct is_negative< Degrees<N> >
:
        boost::mpl::not_< is_positive< Degrees<N> > >
{};

}       // namespace angle
}       // namespace dctl
