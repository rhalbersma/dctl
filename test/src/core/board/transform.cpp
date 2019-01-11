//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/group.hpp>         // is_idempotent, is_identity, is_involution
#include <dctl/core/board/angle.hpp>    // _deg, inverse, rotate, mirror
#include <boost/range/irange.hpp>       // irange
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <algorithm>                    // all_of

using namespace dctl::core;
using namespace literals;

BOOST_AUTO_TEST_SUITE(angleTransform)

auto angles = boost::irange(-2 * 360, 2 * 360 + 1);

BOOST_AUTO_TEST_CASE(AngleConstructorIsIdempotentOnIntegers)
{
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto i) {
                        return group::is_idempotent{}([](auto j) { return j; }, angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(RotateZeroDegIsIdentityOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto i) {
                        return group::is_identity{}([](auto j) { return rotate(j, 0_deg); }, angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(ReverseIsInvolutionOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto i) {
                        return group::is_involution{}([](auto j) { return reverse(j); }, angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(InverseIsInvolutionOnAllAngles)
{
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto i) {
                        return group::is_involution{}([](auto j) { return inverse(j); }, angle{i});
                })
        );
}

BOOST_AUTO_TEST_CASE(MirrorIsInvolutionOnAllanglePairs)
{
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto i) {
                        return std::all_of(angles.begin(), angles.end(), [=](auto j) {
                                return group::is_involution{}([=](auto k) { return mirror(k, angle{j}); }, angle{i});
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()
