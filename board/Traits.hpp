#pragma once
#include <boost/mpl/arithmetic.hpp>     // divides, modulus
#include <boost/mpl/comparison.hpp>     // equal_to, not_equal_to
#include <boost/mpl/int_fwd.hpp>        // int_
#include <boost/mpl/logical.hpp>        // not_, and_, or_
#include "Traits_fwd.hpp"
#include "Angle.hpp"                    // angle, rotate
#include "Degrees.hpp"                  // L090, D090, D180
#include "Transform.hpp"                // rotate

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

// 0, 2, 4, 6
template<int N>
struct is_orthogonal< Angle<N> >
:
        boost::mpl::equal_to<
                boost::mpl::modulus<
                        boost::mpl::int_<N>,
                        boost::mpl::int_<D090::value>
                >,
                boost::mpl::int_<0>
        >
{};

// 1, 3, 5, 7
template<int N>
struct is_diagonal< Angle<N> >
:
        boost::mpl::not_< is_orthogonal< Angle<N> > >
{};

// 1, 2, 3
template<int N>
struct is_up< Angle<N> >
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

// 5, 6, 7
template<int N>
struct is_down< Angle<N> >
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

// 3, 4, 5
template<int N>
struct is_left< Angle<N> >
:
        is_down< rotate< Angle<N>, L090 > >
{};

// 0, 1, 7
template<int N>
struct is_right< Angle<N> >
:
        is_up< rotate< Angle<N>, L090 > >
{};

// 1, 2, 3, 4
template<int N>
struct is_positive< Angle<N> >
:
        boost::mpl::or_<
                is_up< Angle<N> >,
                boost::mpl::and_<
                        is_left< Angle<N> >,
                        boost::mpl::not_<
                                is_down< Angle<N> >
                        >
                >
        >
{};

// 0, 5, 6, 7
template<int N>
struct is_negative< Angle<N> >
:
        boost::mpl::not_< is_positive< Angle<N> > >
{};

}       // namespace angle
}       // namespace dctl
