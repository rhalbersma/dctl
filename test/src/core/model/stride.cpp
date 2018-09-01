//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/model/detail/stride.hpp>    // stride
#include <core/board/sequence.hpp>              // nano, micro, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, Compact_10_12, Compact_12_10, basic_board<12, 10>, canadian, srilankan, dumm
#include <dctl/core/board/angle.hpp>            // angle, reverse
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_EQUAL
#include <algorithm>                            // all_of
#include <vector>                               // vector

using namespace dctl::core;
using namespace literals;

BOOST_AUTO_TEST_SUITE(ActionsStride)

BOOST_AUTO_TEST_CASE_TEMPLATE(ReverseAngleSymmetryForAllDirections, T, BoardSequence)
{
        auto const directions = std::vector<angle>
        {
                   0_deg,  45_deg,  90_deg, 135_deg,
                 180_deg, 225_deg, 270_deg, 315_deg
        };

        BOOST_CHECK(
                std::all_of(directions.begin(), directions.end(), [](auto dir) {
                        return stride<T>{}(dir) == stride<T>{}(reverse(dir));
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()
