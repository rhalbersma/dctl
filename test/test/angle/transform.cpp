#include <algorithm>                            // all_of
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle.hpp>                       // _deg, inverse, rotate, mirror
#include <dctl/group/transform.hpp>             // IsIdempotent, IsIdentity, IsInvolution

namespace dctl {

BOOST_AUTO_TEST_SUITE(AngleTransform)

BOOST_AUTO_TEST_CASE(AngleConstructorIsIdempotentOnIntegers)
{
        auto const first = boost::counting_iterator<int>{-2 * 360    };
        auto const last  = boost::counting_iterator<int>{ 2 * 360 + 1};

        BOOST_CHECK(
                std::all_of(first, last, [](auto i){
                        return group::IsIdempotent()([](auto j) { return Angle{j}; }, i);
                })
        );
}

auto const alpha = boost::counting_iterator<int>{  0};
auto const omega = boost::counting_iterator<int>{360};

BOOST_AUTO_TEST_CASE(AngleConstructorIsIdentityOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto i){
                        return group::IsIdentity()([](auto j) { return Angle{j}; }, i);
                })
        );
}

BOOST_AUTO_TEST_CASE(Rotate0DegIsIdentityOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto i){
                        return group::IsInvolution()([](auto j) { return rotate(Angle{j}, 0_deg); }, i);
                })
        );
}

BOOST_AUTO_TEST_CASE(InverseIsInvolutionOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(alpha, omega, [](auto i){
                        return group::IsInvolution()([](auto j) { return inverse(Angle{j}); }, i);
                })
        );
}

BOOST_AUTO_TEST_CASE(Rotate180DegIsInvolutionOnAllAngles)
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
                        return std::all_of(alpha, omega, [=](auto j) {
                                return group::IsInvolution()([=](auto k) { return mirror(Angle{k}, Angle{i}); }, j);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
