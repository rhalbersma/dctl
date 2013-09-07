#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/degrees.hpp>       // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/transform.hpp>     // inverse

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleDegrees)

BOOST_AUTO_TEST_CASE(LeftIsInverseRight)
{
        BOOST_CHECK_EQUAL(L000, inverse(R000));
        BOOST_CHECK_EQUAL(L045, inverse(R045));
        BOOST_CHECK_EQUAL(L090, inverse(R090));
        BOOST_CHECK_EQUAL(L135, inverse(R135));
        BOOST_CHECK_EQUAL(L180, inverse(R180));
}

BOOST_AUTO_TEST_CASE(RightIsInverseLeft)
{
        BOOST_CHECK_EQUAL(R000, inverse(L000));
        BOOST_CHECK_EQUAL(R045, inverse(L045));
        BOOST_CHECK_EQUAL(R090, inverse(L090));
        BOOST_CHECK_EQUAL(R135, inverse(L135));
        BOOST_CHECK_EQUAL(R180, inverse(L180));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
