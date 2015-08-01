#include <board/group.hpp>                    // is_idempotent, is_identity, is_involution
#include <dctl/board/angle.hpp>               // _deg, inverse, rotate, mirror
#include <range/v3/all.hpp>             // all_of, view::iota
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK

namespace dctl {

BOOST_AUTO_TEST_SUITE(AngleTransform)

BOOST_AUTO_TEST_CASE(AngleConstructorIsIdempotentOnIntegers)
{
        auto const angles = ranges::view::iota(-2 * 360, 2 * 360 + 1);

        BOOST_CHECK(
                ranges::all_of(angles, [](auto i){
                        return group::is_idempotent{}([](auto j) { return Angle{j}; }, i);
                })
        );
}

auto const angles = ranges::view::iota(0, 360);

BOOST_AUTO_TEST_CASE(AngleConstructorIsIdentityOnAllAngles)
{
        BOOST_CHECK(
                ranges::all_of(angles, [](auto i){
                        return group::is_identity{}([](auto j) { return Angle{j}; }, i);
                })
        );
}

BOOST_AUTO_TEST_CASE(Rotate0DegIsIdentityOnAllAngles)
{
        BOOST_CHECK(
                ranges::all_of(angles, [](auto i){
                        return group::is_involution{}([](auto j) { return rotate(Angle{j}, 0_deg); }, i);
                })
        );
}

BOOST_AUTO_TEST_CASE(InverseIsInvolutionOnAllAngles)
{
        BOOST_CHECK(
                ranges::all_of(angles, [](auto i){
                        return group::is_involution{}([](auto j) { return inverse(Angle{j}); }, i);
                })
        );
}

BOOST_AUTO_TEST_CASE(Rotate180DegIsInvolutionOnAllAngles)
{
        BOOST_CHECK(
                ranges::all_of(angles, [](auto i){
                        return group::is_involution{}([](auto j) { return rotate(Angle{j}, 180_deg); }, i);
                })
        );
}

BOOST_AUTO_TEST_CASE(MirrorIsInvolutionOnAllAnglePairs)
{
        BOOST_CHECK(
                ranges::all_of(angles, [](auto i){
                        return ranges::all_of(angles, [=](auto j) {
                                return group::is_involution{}([=](auto k) { return mirror(Angle{k}, Angle{i}); }, j);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
