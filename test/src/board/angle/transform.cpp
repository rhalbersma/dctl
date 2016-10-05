#include <board/group.hpp>                      // is_idempotent, is_identity, is_involution
#include <dctl/board/angle.hpp>                 // _deg, inverse, rotate, mirror
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of
#include <boost/range/irange.hpp>               // irange
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK

using namespace dctl::board;

BOOST_AUTO_TEST_SUITE(AngleTransform)

auto const angles = boost::irange(-2 * 360, 2 * 360 + 1);

BOOST_AUTO_TEST_CASE(AngleConstructorIsIdempotentOnIntegers)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return group::is_idempotent{}([](auto const j) { return Angle{j.degrees()}; }, Angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(RotateZeroDegIsIdentityOnAllangles)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return group::is_identity{}([](auto const j) { return rotate(j, 0_deg); }, Angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(ReverseIsInvolutionOnAllangles)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return group::is_involution{}([](auto const j) { return reverse(j); }, Angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(InverseIsInvolutionOnAllangles)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return group::is_involution{}([](auto const j) { return inverse(j); }, Angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(MirrorIsInvolutionOnAllAnglePairs)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return boost::algorithm::all_of(angles, [=](auto const j) {
                                return group::is_involution{}([=](auto const k) { return mirror(k, Angle{j}); }, Angle{i});
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()
