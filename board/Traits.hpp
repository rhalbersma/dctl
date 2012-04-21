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

// 1, 3, 5, 7
template<int N>
struct is_diagonal< angle<N> >
:
        boost::mpl::not_< is_orthogonal< angle<N> > >
{};

// 1, 2, 3
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

// 5, 6, 7
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

// 3, 4, 5
template<int N>
struct is_left< angle<N> >
:
        is_down< rotate< angle<N>, angle<degrees::L090> > >
{};

// 0, 1, 7
template<int N>
struct is_right< angle<N> >
:
        is_up< rotate< angle<N>, angle<degrees::L090> > >
{};

// 1, 2, 3, 4
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

// 0, 5, 6, 7
template<int N>
struct is_negative< angle<N> >
:
        boost::mpl::not_< is_positive< angle<N> > >
{};

}       // namespace board
}       // namespace dctl
