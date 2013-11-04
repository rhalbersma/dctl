#include <algorithm>                            // any_of
#include <functional>                           // bind, _1
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/degrees.hpp>               // _deg2
#include <dctl/angle/transform.hpp>             // inverse, rotate, mirror
#include <dctl/group/transform.hpp>             // IsInvolution

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleTransform)

using namespace std::placeholders;      // _1

auto const alpha = boost::counting_iterator<int>{  0};
auto const omega = boost::counting_iterator<int>{360};

BOOST_AUTO_TEST_CASE(InverseIsInvolutionOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto const& i){
                        return group::IsInvolution()(std::bind(dctl::inverse2, _1), Angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(Rotate180IsInvolutionOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto const& i){
                        return group::IsInvolution()(std::bind(dctl::rotate2, _1, 180_deg2), Angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(MirrorIsInvolutionOnAllAnglePairs)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto const& i){
                        return std::all_of(alpha, omega, [=](int const& j) {
                                return group::IsInvolution()(std::bind(dctl::mirror2, _1, Angle{i}), Angle{j});
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
