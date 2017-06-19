//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>                 // angle, _deg, is_orthogonal, is_diagonal, is_up, is_down, is_left, is_right, is_positive, is_negative
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <vector>                               // vector

using namespace dctl::core;
using namespace literals;

BOOST_AUTO_TEST_SUITE(angleTraits)

BOOST_AUTO_TEST_CASE(OrthogonalIsNotDiagonal)
{
        auto const angles = std::vector<angle>{ 0_deg, 90_deg, 180_deg, 270_deg };
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const& a) {
                        return is_orthogonal(a) && not is_diagonal(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(DiagonalIsNotOrthogonal)
{
        auto const angles = std::vector<angle>{ 45_deg, 135_deg, 225_deg, 315_deg };
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const& a) {
                        return is_diagonal(a) && not is_orthogonal(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(UpIsNotDown)
{
        auto const angles = std::vector<angle>{ 45_deg, 90_deg, 135_deg };
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const& a) {
                        return is_up(a) && not is_down(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(DownIsNotUp)
{
        auto const angles = std::vector<angle>{ 225_deg, 270_deg, 315_deg };
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const& a) {
                        return is_down(a) && not is_up(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(LeftIsNotRight)
{
        auto const angles = std::vector<angle>{ 135_deg, 180_deg, 225_deg };
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const& a) {
                        return is_left(a) && not is_right(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(RightIsNotLeft)
{
        auto const angles = std::vector<angle>{ 315_deg, 0_deg, 45_deg };
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const& a) {
                        return is_right(a) && not is_left(a);
                })
        );
}
/*
BOOST_AUTO_TEST_CASE(PositiveIsNotNegative)
{
        auto const angles = std::vector<angle>{ 45_deg, 90_deg, 135_deg, 180_deg };
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const& a) {
                        return is_positive(a) && not is_negative(a);
                })
        );
}

BOOST_AUTO_TEST_CASE(NegativeIsNotPositive)
{
        auto const angles = std::vector<angle>{ 225_deg, 270_deg, 315_deg, 0_deg };
        BOOST_CHECK(
                boost::algorithm::all_of(angles, [](auto const& a) {
                        return is_negative(a) && not is_positive(a);
                })
        );
}
*/
BOOST_AUTO_TEST_SUITE_END()
