#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <dctl/angle/detail/abs_remainder.hpp>  // abs, abs_remainder

namespace dctl {

BOOST_AUTO_TEST_SUITE(AngleAbsRemainder)

BOOST_AUTO_TEST_CASE(AbsRemainderIsLessThanAbsDivisor)
{
        BOOST_CHECK_LT(detail::abs_remainder(-2, 2), detail::abs( 2));
        BOOST_CHECK_LT(detail::abs_remainder(-1, 2), detail::abs( 2));
        BOOST_CHECK_LT(detail::abs_remainder( 0, 2), detail::abs( 2));
        BOOST_CHECK_LT(detail::abs_remainder( 1, 2), detail::abs( 2));
        BOOST_CHECK_LT(detail::abs_remainder( 2, 2), detail::abs( 2));

        BOOST_CHECK_LT(detail::abs_remainder(-2,-2), detail::abs(-2));
        BOOST_CHECK_LT(detail::abs_remainder(-1,-2), detail::abs(-2));
        BOOST_CHECK_LT(detail::abs_remainder( 0,-2), detail::abs(-2));
        BOOST_CHECK_LT(detail::abs_remainder( 1,-2), detail::abs(-2));
        BOOST_CHECK_LT(detail::abs_remainder( 2,-2), detail::abs(-2));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
