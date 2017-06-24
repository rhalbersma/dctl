//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algo/search/fixture.hpp>
#include <dctl/core.hpp>
#include <boost/test/unit_test.hpp>

using namespace dctl::algo;
using namespace dctl::core;

using F = search::Fixture< state<international>, search::DefaultObjective >;

BOOST_AUTO_TEST_SUITE(Searchinitial)

BOOST_FIXTURE_TEST_CASE(I, F)
{
        auto const p = state<rules_t<F>, board_t<F>>::initial();
        root_.analyze(drop_duplicates_gen, p, 71);
}

BOOST_AUTO_TEST_SUITE_END()
