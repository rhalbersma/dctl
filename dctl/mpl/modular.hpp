#pragma once
#include <boost/assert.hpp>             // BOOST_STATIC_ASSERT
#include <boost/mpl/arithmetic.hpp>     // modulus, plus
#include <boost/mpl/comparison.hpp>     // less
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/int.hpp>            // int_

namespace dctl {
namespace mpl {

template
<
        typename Numerator,
        typename Denominator
>
struct abs_modulus
:
        boost::mpl::modulus< typename
                boost::mpl::eval_if<
                        boost::mpl::less<
                                Numerator,
                                boost::mpl::int_<0>
                        >,
                        boost::mpl::plus<
                                boost::mpl::modulus<
                                        Numerator,
                                        Denominator
                                >,
                                Denominator
                        >,
                        Numerator
                >::type,
                Denominator
        >
{
        BOOST_STATIC_ASSERT(0 <= (abs_modulus::value) && (abs_modulus::value) < Denominator::value);
};

}	// namespace mpl
}       // namespace dctl