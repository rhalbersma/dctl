#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_AUTO_TEST_SUITE_END
#include <dctl/utility/abs.hpp>                 // abs, abs_remainder

namespace dctl {
namespace util {

BOOST_AUTO_TEST_SUITE(UtilityAbs)

BOOST_AUTO_TEST_CASE(AbsZeroIsZero)
{
        BOOST_CHECK_EQUAL(util::abs(0), 0);
}

BOOST_AUTO_TEST_CASE(AbsNonZeroIsPositive)
{
        BOOST_CHECK_GT(util::abs(-1), 0);
        BOOST_CHECK_GT(util::abs( 1), 0);
}

BOOST_AUTO_TEST_CASE(AbsRemainderIsLessThanAbsDivisor)
{
        BOOST_CHECK_LT(abs_remainder(-2, 2), abs( 2));
        BOOST_CHECK_LT(abs_remainder(-1, 2), abs( 2));
        BOOST_CHECK_LT(abs_remainder( 0, 2), abs( 2));
        BOOST_CHECK_LT(abs_remainder( 1, 2), abs( 2));
        BOOST_CHECK_LT(abs_remainder( 2, 2), abs( 2));

        BOOST_CHECK_LT(abs_remainder(-2,-2), abs(-2));
        BOOST_CHECK_LT(abs_remainder(-1,-2), abs(-2));
        BOOST_CHECK_LT(abs_remainder( 0,-2), abs(-2));
        BOOST_CHECK_LT(abs_remainder( 1,-2), abs(-2));
        BOOST_CHECK_LT(abs_remainder( 2,-2), abs(-2));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace util
}       // namespace dctl
