//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>    // angle, _deg, is_orthogonal, is_diagonal, is_up, is_down, is_left, is_right, is_positive, is_negative
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <algorithm>                    // all_of
#include <vector>                       // vector

using namespace dctl::core;
using namespace literals;

BOOST_AUTO_TEST_SUITE(angleTraits)

BOOST_AUTO_TEST_CASE(OrthogonalIsNotDiagonal)
{
        auto const angles = std::vector<angle>{ 0_deg, 90_deg, 180_deg, 270_deg };
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto const& a) {
                        return is_orthogonal(a) && !is_diagonal(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(DiagonalIsNotOrthogonal)
{
        auto const angles = std::vector<angle>{ 45_deg, 135_deg, 225_deg, 315_deg };
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto const& a) {
                        return is_diagonal(a) && !is_orthogonal(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(UpIsNotDown)
{
        auto const angles = std::vector<angle>{ 45_deg, 90_deg, 135_deg };
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto const& a) {
                        return is_up(a) && !is_down(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(DownIsNotUp)
{
        auto const angles = std::vector<angle>{ 225_deg, 270_deg, 315_deg };
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto const& a) {
                        return is_down(a) && !is_up(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(LeftIsNotRight)
{
        auto const angles = std::vector<angle>{ 135_deg, 180_deg, 225_deg };
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto const& a) {
                        return is_left(a) && !is_right(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(RightIsNotLeft)
{
        auto const angles = std::vector<angle>{ 315_deg, 0_deg, 45_deg };
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto const& a) {
                        return is_right(a) && !is_left(a);
                })
        );
}
/*
BOOST_AUTO_TEST_CASE(PositiveIsNotNegative)
{
        auto const angles = std::vector<angle>{ 45_deg, 90_deg, 135_deg, 180_deg };
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto const& a) {
                        return is_positive(a) && !is_negative(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(NegativeIsNotPositive)
{
        auto const angles = std::vector<angle>{ 225_deg, 270_deg, 315_deg, 0_deg };
        BOOST_CHECK(
                std::all_of(angles.begin(), angles.end(), [](auto const& a) {
                        return is_negative(a) && !is_positive(a);
                })
        );
}
*/
BOOST_AUTO_TEST_SUITE_END()
