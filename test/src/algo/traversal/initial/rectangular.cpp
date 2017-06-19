
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

BOOST_AUTO_TEST_SUITE(Walkinitialrectangular)

BOOST_AUTO_TEST_CASE(spantsiretti)
{
        auto const p = state<international, board::spantsiretti>::initial();
        auto const leafs = std::vector<int64_t>{ 9, 81, 658, 4'265, 27'117, 167'140, 1'029'319, 6'127'190, 36'751'086, 218'398'167, 1'306'818'894 };
        traversal::test(drop_duplicates_gen, p, leafs);
}

BOOST_AUTO_TEST_CASE(ktar10x11)
{
        auto const p = state<international, board::ktar<10,11>>::initial();
        auto const leafs = std::vector<int64_t>{ 9, 81, 810, 7'583, 74'602, 688'835, 6'555'302, 59'796'721, 558'094'366, 5'058'805'542 };
        traversal::test(drop_duplicates_gen, p, leafs);
}

// The "Perft on rectangular boards" thread on the FMJD forum (results until depth=9)
// http://laatste.info/bb3/viewtopic.php?f=53&t=3014
// The "Perft on large boards" thread on the FMJD forum (results until depth=11)
// http://laatste.info/bb3/viewtopic.php?f=53&t=4563&start=2

BOOST_AUTO_TEST_CASE(ktar10x12)
{
        auto const leafs = std::vector<int64_t>{ 9, 81, 658, 4'265, 27'117, 167'140, 1'049'442, 6'483'961, 41'291'394, 263'895'730, 1'731'541'289 };

        // board with international initial moves and orthogonal capture support: does not fit into 64 bits
        auto const q = state<international, board::ktar<10,12>>::initial();
        traversal::test(drop_duplicates_gen, q, leafs);
}

// The "Perft on large boards" thread on the FMJD forum (results until depth=10)
// http://laatste.info/bb3/viewtopic.php?f=53&t=4563&start=1

BOOST_AUTO_TEST_CASE(Board12x10)
{
        auto const leafs = std::vector<int64_t>{ 11, 121, 1'222, 10'053, 79'049, 584'100, 4'369'366, 31'839'056, 236'364'607, 1'742'748'504 };

        // board with canadian initial moves and orthogonal capture support: does not fit into 64 bits
        auto const q = state<international, board::rectangular<12,10>>::initial();
        traversal::test(drop_duplicates_gen, q, leafs);
}

BOOST_AUTO_TEST_SUITE_END()
