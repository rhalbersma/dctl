#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/angle.hpp>         // is_angle, make_angle

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleAngle)

BOOST_AUTO_TEST_CASE(MakeAngleIsAngle)
{
        for (auto i = -720; i <= 720; ++i)
                BOOST_CHECK(is_angle(make_angle(i)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
