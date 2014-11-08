#include <dctl/angle.hpp>               // _deg, inverse
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleDegrees)

BOOST_AUTO_TEST_CASE(LeftIsInverseOfRight)
{
        BOOST_CHECK_EQUAL(  +0_deg, inverse(  -0_deg));
        BOOST_CHECK_EQUAL( +45_deg, inverse( -45_deg));
        BOOST_CHECK_EQUAL( +90_deg, inverse( -90_deg));
        BOOST_CHECK_EQUAL(+135_deg, inverse(-135_deg));
        BOOST_CHECK_EQUAL(+180_deg, inverse(-180_deg));
}

BOOST_AUTO_TEST_CASE(RightIsInverseOfLeft)
{
        BOOST_CHECK_EQUAL(  -0_deg, inverse(  +0_deg));
        BOOST_CHECK_EQUAL( -45_deg, inverse( +45_deg));
        BOOST_CHECK_EQUAL( -90_deg, inverse( +90_deg));
        BOOST_CHECK_EQUAL(-135_deg, inverse(+135_deg));
        BOOST_CHECK_EQUAL(-180_deg, inverse(+180_deg));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
