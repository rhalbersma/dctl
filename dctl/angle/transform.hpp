#pragma once
#include <boost/mpl/arithmetic.hpp>     // negate, plus
#include <boost/mpl/int.hpp>            // int_
#include <dctl/angle/Degrees.hpp>       // Degrees
#include <dctl/angle/transform_fwd.hpp> // inverse, rotate, mirror (partial specialization declarations)
#include <dctl/mpl/modular.hpp>         // abs_modulus

namespace dctl {
namespace mpl {
namespace lazy {

// NOTE: because inverse, rotate and mirror are LAZY metafunctions, 
// their primary template definitions need to have been seen at this point

// partial specialization definitions

template<int N>
struct inverse< angle::Degrees<N> >
:
        angle::Degrees<  
                boost::mpl::negate<
                        boost::mpl::int_<N>
                >::value
        >
{};

template<int N1, int N2>
struct rotate< angle::Degrees<N1>, angle::Degrees<N2> >
:
        angle::Degrees<  
                boost::mpl::plus<
                        boost::mpl::int_<N1>,
                        boost::mpl::int_<N2>
                >::value
        >
{};

template<int N1, int N2>
struct mirror< angle::Degrees<N1>, angle::Degrees<N2> >
:
        rotate<
                inverse<
                        rotate<
                                angle::Degrees<N1>,
                                inverse< angle::Degrees<N2> >
                        >
                >,
                angle::Degrees<N2>
        >
{};

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
