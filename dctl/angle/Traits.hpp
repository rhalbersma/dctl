#pragma once
#include <boost/mpl/arithmetic.hpp>     // divides, modulus
#include <boost/mpl/comparison.hpp>     // equal_to, not_equal_to
#include <boost/mpl/int.hpp>            // int_
#include <boost/mpl/logical.hpp>        // not_, and_, or_
#include <dctl/angle/Degrees.hpp>       // L090, D090, D180
#include <dctl/angle/traits_fwd.hpp>    // is_orthogonal, is_diagonal, is_up, is_down, is_left, is_right, is_positive, is_negative (primary template definitions)
#include <dctl/angle/transform.hpp>	// rotate

namespace dctl {
namespace angle {
namespace lazy {

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

// NOTE: because is_orthogonal, is_diagonal, is_up, is_down, is_left, is_right, is_positive, is_negative 
// are LAZY metafunctions, their primary template definitions need to have been seen at this point

// partial specialization definitions

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
        is_down< mpl::lazy::rotate< Degrees<N>, L090 > >
{};

template<int N>
struct is_right< Degrees<N> >
:
        is_up< mpl::lazy::rotate< Degrees<N>, L090 > >
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

}       // namespace lazy
}       // namespace angle
}       // namespace dctl
