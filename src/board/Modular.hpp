#pragma once
#include <boost/mpl/arithmetic.hpp>     // modulus, plus
#include <boost/mpl/comparison.hpp>     // less
#include <boost/mpl/eval_if.hpp>        // eval_if
#include <boost/mpl/int.hpp>            // int_
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT

namespace dctl {

template<
        typename Numerator,
        typename Denominator
>
struct abs_modulus
:
        boost::mpl::modulus<
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
                >,
                Denominator
        >
{
        // the remainder is non-negative and less than the denominator
        /*BOOST_STATIC_ASSERT(
                0 <= (abs_modulus<Numerator, Denominator>::value) &&
                (abs_modulus<Numerator, Denominator>::value) < Denominator::value
        );*/
};

}       // namespace dctl
