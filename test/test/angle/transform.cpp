#include <algorithm>                            // all_of
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/degrees.hpp>               // _deg
#include <dctl/angle/transform.hpp>             // inverse, rotate, mirror
#include <dctl/group/transform.hpp>             // IsInvolution

namespace dctl {

BOOST_AUTO_TEST_SUITE(AngleTransform)

using namespace std::placeholders;      // _1

auto const alpha = boost::counting_iterator<int>{  0};
auto const omega = boost::counting_iterator<int>{360};

BOOST_AUTO_TEST_CASE(InverseIsInvolutionOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto i){
                        return group::IsInvolution()([](auto j) { return inverse(Angle{j}); }, i);
                })
        );
}

BOOST_AUTO_TEST_CASE(Rotate180IsInvolutionOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto i){
                        return group::IsInvolution()([](auto j) { return rotate(Angle{j}, 180_deg); }, i);
                })
        );
}

BOOST_AUTO_TEST_CASE(MirrorIsInvolutionOnAllAnglePairs)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto i){
                        return std::all_of(alpha, omega, [=](int j) {
                                return group::IsInvolution()([=](int k) { return mirror(Angle{k}, Angle{i}); }, j);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
