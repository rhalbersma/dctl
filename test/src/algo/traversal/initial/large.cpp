
//          Copyright Rein Halbersma 2010-2017.
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

BOOST_AUTO_TEST_SUITE(WalkinitialLarge)

// The "Perft on large boards" thread on the FMJD forum (results until depth=10)
// http://laatste.info/bb3/viewtopic.php?f=53&t=4563&start=3

BOOST_AUTO_TEST_CASE(Board12x12)
{
        auto const leafs = std::vector<int64_t>{ 11, 121, 1'222, 10'053, 79'049, 584'100, 4'369'366, 31'839'056, 237'209'258, 1'761'652'936 };

        auto const p = state<canadian, board::canadian>::initial();
        traversal::test(drop_duplicates_gen, p, leafs);

        auto const q = state<srilankan, board::srilankan>::initial();
        traversal::test(drop_duplicates_gen, q, leafs);
}

BOOST_AUTO_TEST_CASE(Board14x14)
{
        auto const p = state<international, board::dumm>::initial();
        auto const leafs = std::vector<int64_t>{ 13, 169, 2'042, 20'513, 195'333, 1'710'812, 15'007'858, 127'249'292, 1'093'968'733 };

        traversal::test(drop_duplicates_gen, p, leafs);
}

BOOST_AUTO_TEST_SUITE_END()
