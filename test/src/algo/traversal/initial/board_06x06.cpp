//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algo/traversal/fixture.hpp>
#include <dctl/algo/traversal/root.hpp>
#include <dctl/core.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>                      // int64_t
#include <vector>

using namespace dctl::algo;
using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(TraversalInitialBoard06x06)

// The "Perft for arbitrary board sizes" on the FMJD forum
// http://damforum.nl/bb3/viewtopic.php?f=53&t=2666&start=5

BOOST_AUTO_TEST_CASE(International)
{
        auto const state = basic_state<international, basic_board<rectangular<6, 6>>>::initial();
        auto const leafs = std::vector<int64_t>
                { 5, 25, 106, 369, 1'301, 4'214, 12'912, 39'526, 113'614, 329'982, 933'564, 2'678'979, 7'509'079, 21'546'796, 59'298'144, 172'303'864, 480'328'231 };
        traversal::test(state, leafs, drop_duplicates_gen);
}

BOOST_AUTO_TEST_CASE(Checkers)
{
        auto const state = basic_state<checkers, basic_board<rectangular<6, 6>>>::initial();
        auto const leafs = std::vector<int64_t>
                { 5, 25, 106, 369, 1'271, 4'104, 12'495, 37'474, 104'013, 290'032, 808'158, 2'209'549, 6'039'632, 16'775'007, 45'722'008, 129'088'590, 354'735'390 };
        traversal::test(state, leafs, keep_duplicates_gen);
}

BOOST_AUTO_TEST_SUITE_END()
