#include <algorithm>                            // any_of
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/angle.hpp>                 // is_angle, make_angle

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleAngle)

BOOST_AUTO_TEST_CASE(MakeAngleIsAngle)
{
        auto const alpha = boost::counting_iterator<int>{-2 * 360    };
        auto const omega = boost::counting_iterator<int>{ 2 * 360 + 1};

        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto const& i){
                        return is_angle(int{Angle{i}});
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
