
//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/sequence.hpp>                   // micro, mini, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, Compact_10_12, Compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <dctl/core/board/angle.hpp>                 // angle, reverse
#include <dctl/core/board/detail/shift.hpp>                 // shift_size
#include <boost/algorithm/cxx11/all_of.hpp>     // all_of
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK_EQUAL
#include <vector>                               // vector

using namespace dctl::core;
using namespace literals;

BOOST_AUTO_TEST_SUITE(GridShiftSize)

BOOST_AUTO_TEST_CASE_TEMPLATE(ReverseangleSymmetryForAllDirections, T, board::BoardSequence)
{
        auto const directions = std::vector<angle>
        {
                   0_deg,  45_deg,  90_deg, 135_deg,
                 180_deg, 225_deg, 270_deg, 315_deg
        };

        BOOST_CHECK(
                boost::algorithm::all_of(directions, [](auto dir) {
                        return detail::shift_size<T>{}(dir) == detail::shift_size<T>{}(reverse(dir));
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()
