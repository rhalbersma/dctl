//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algo/search/fixture.hpp>
#include <dctl/core.hpp>
#include <boost/test/unit_test.hpp>

using namespace dctl::algo;
using namespace dctl::core;

using F = search::Fixture< basic_state<international, basic_board<international>>, search::DefaultObjective >;

BOOST_AUTO_TEST_SUITE(SearchInitialInternational)

BOOST_FIXTURE_TEST_CASE(I, F)
{
        auto const s = basic_state<rules_t<F>, board_t<F>>::initial();
        root_.analyze(drop_duplicates_gen, s, 71);
}

BOOST_AUTO_TEST_SUITE_END()
