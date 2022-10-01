//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>    // angle, _deg, is_orthogonal, is_diagonal, is_up, is_down, is_left, is_right, is_positive, is_negative
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <algorithm>                    // all_of
#include <array>                        // array

using namespace dctl::core;
using namespace literals;

BOOST_AUTO_TEST_SUITE(AngleTraits)

BOOST_AUTO_TEST_CASE(IsUp)
{
        auto const angles = std::array{ 45_deg, 90_deg, 135_deg };
        BOOST_CHECK(
                std::ranges::all_of(angles, [](auto a) {
                        return is_up(a);
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()
