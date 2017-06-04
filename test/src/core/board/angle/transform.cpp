#include <core/board/group.hpp>                      // is_idempotent, is_identity, is_involution
#include <dctl/core/board/angle.hpp>                 // _deg, inverse, rotate, mirror
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of
#include <boost/range/irange.hpp>               // irange
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK

using namespace dctl::core;
using namespace board::literals;

BOOST_AUTO_TEST_SUITE(angleTransform)

auto const angles = boost::irange(-2 * 360, 2 * 360 + 1);

BOOST_AUTO_TEST_CASE(angleConstructorIsIdempotentOnIntegers)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return board::group::is_idempotent{}([](auto const j) { return j; }, board::angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(RotateZeroDegIsIdentityOnAllangles)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return board::group::is_identity{}([](auto const j) { return rotate(j, 0_deg); }, board::angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(ReverseIsInvolutionOnAllangles)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return board::group::is_involution{}([](auto const j) { return reverse(j); }, board::angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(InverseIsInvolutionOnAllangles)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return board::group::is_involution{}([](auto const j) { return inverse(j); }, board::angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(MirrorIsInvolutionOnAllanglePairs)
{
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const i){
                        return boost::algorithm::all_of(angles, [=](auto const j) {
                                return board::group::is_involution{}([=](auto const k) { return mirror(k, board::angle{j}); }, board::angle{i});
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()
