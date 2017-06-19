//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/traversal/root.hpp>
#include <dctl/core.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>                      // int64_t
#include <vector>

#include <algo/traversal/fixture.hpp>

using namespace dctl::algo;
using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(Walkinternationalshashki)

// The "Standard validation generator moves" thread on the shashki forum
// http://shashki.com/PNphpBB2-viewtopic-t-627-start-58.html

BOOST_AUTO_TEST_CASE(Frontrows)
{
        auto const p = setup::read<international, board::international, pdn::protocol>()(
                "W:B16,17,18,19,20:W31,32,33,34,35"
        );
        auto const leafs = std::vector<int64_t>
                { 9, 81, 534, 2'538, 9'052, 29'999, 98'971, 310'315, 1'008'317, 3'174'159, 10'369'533, 34'907'917, 123'162'301, 462'333'827, 1'824'763'597 };
        traversal::test(keep_duplicates_gen, p, leafs);
}

BOOST_AUTO_TEST_SUITE_END()
