#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL
#include <dctl/angle/degrees.hpp>       // _deg2
#include <dctl/angle/transform.hpp>     // inverse

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleDegrees)

BOOST_AUTO_TEST_CASE(LeftIsInverseOfRight)
{
        BOOST_CHECK_EQUAL(  +0_deg2, inverse2(  -0_deg2));
        BOOST_CHECK_EQUAL( +45_deg2, inverse2( -45_deg2));
        BOOST_CHECK_EQUAL( +90_deg2, inverse2( -90_deg2));
        BOOST_CHECK_EQUAL(+135_deg2, inverse2(-135_deg2));
        BOOST_CHECK_EQUAL(+180_deg2, inverse2(-180_deg2));
}

BOOST_AUTO_TEST_CASE(RightIsInverseOfLeft)
{
        BOOST_CHECK_EQUAL(  -0_deg2, inverse2(  +0_deg2));
        BOOST_CHECK_EQUAL( -45_deg2, inverse2( +45_deg2));
        BOOST_CHECK_EQUAL( -90_deg2, inverse2( +90_deg2));
        BOOST_CHECK_EQUAL(-135_deg2, inverse2(+135_deg2));
        BOOST_CHECK_EQUAL(-180_deg2, inverse2(+180_deg2));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
