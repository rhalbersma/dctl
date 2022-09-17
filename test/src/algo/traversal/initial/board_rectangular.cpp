//          Copyright Rein Halbersma 2010-2022.
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

BOOST_AUTO_TEST_SUITE(TraversalInitialBoardRectangular)

BOOST_AUTO_TEST_CASE(Spantsiretti)
{
        auto const s = basic_state<international, basic_board<spantsiretti>>::initial();
        auto const leafs = std::vector<int64_t>
                { 9, 81, 658, 4'265, 27'117, 167'140, 1'029'319, 6'127'190, 36'751'086, 218'398'167, 1'306'818'894 };
        traversal::test(model::drop_duplicates_gen, s, leafs);
}

BOOST_AUTO_TEST_CASE(Ktar08x10)
{
        auto const s = basic_state<international, basic_board<ktar< 8, 10>>>::initial();
        auto const leafs = std::vector<int64_t>
                { 7, 49, 302, 1'469, 7'473, 37'628, 193'605, 995'303, 5'222'464, 27'405'228, 146'853'805 };
        traversal::test(model::drop_duplicates_gen, s, leafs);
}

BOOST_AUTO_TEST_CASE(Ktar10x11)
{
        auto const s = basic_state<international, basic_board<ktar<10,11>>>::initial();
        auto const leafs = std::vector<int64_t>
                { 9, 81, 810, 7'583, 74'602, 688'835, 6'555'302, 59'796'721, 558'094'366, 5'058'805'542 };
        traversal::test(model::drop_duplicates_gen, s, leafs);
}

// The "Perft on rectangular boards" thread on the FMJD forum (results until depth=9)
// http://damforum.nl/bb3/viewtopic.php?f=53&t=3014

// The "Perft on large boards" thread on the FMJD forum (results until depth=11)
// http://damforum.nl/bb3/viewtopic.php?f=53&t=4563&start=2

BOOST_AUTO_TEST_CASE(Ktar10x12)
{
        auto const s = basic_state<international, basic_board<ktar<10,12>>>::initial();
        auto const leafs = std::vector<int64_t>
                { 9, 81, 658, 4'265, 27'117, 167'140, 1'049'442, 6'483'961, 41'291'394, 263'895'730, 1'731'541'289 };
        traversal::test(model::drop_duplicates_gen, s, leafs);
}

// The "Perft on large boards" thread on the FMJD forum (results until depth=10)
// http://damforum.nl/bb3/viewtopic.php?f=53&t=4563&start=1

BOOST_AUTO_TEST_CASE(Board12x10)
{
        auto const s = basic_state<international, basic_board<rectangular<12,10>>>::initial();
        auto const leafs = std::vector<int64_t>
                { 11, 121, 1'222, 10'053, 79'049, 584'100, 4'369'366, 31'839'056, 236'364'607, 1'742'748'504 };
        traversal::test(model::drop_duplicates_gen, s, leafs);
}

BOOST_AUTO_TEST_SUITE_END()
