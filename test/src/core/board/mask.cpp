//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/sequence.hpp>              // nano, micro, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                // ktar<10, 12>, Compact_10_12, Compact_12_10, basic_board<12, 10>, canadian, srilankan, dumm
#include <dctl/util/type_traits.hpp>            // set_t
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                            // all_of
#include <numeric>                              // accumulate
#include <vector>                               // vector

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(TestBoard)

BOOST_AUTO_TEST_CASE_TEMPLATE(jump_groupsEquivalencePartitionSquares, T, BoardSequence)
{
        auto const jump_groups = std::vector<int>{ 0, 1, 2, 3 };

        BOOST_CHECK(
                std::all_of(jump_groups.begin(), jump_groups.end(), [=](auto i) {
                        return std::all_of(jump_groups.begin(), jump_groups.end(), [=](auto j) {
                                return i == j ? true : disjoint(T::jump_group(i), T::jump_group(j));
                        });
                })
        );

        BOOST_CHECK(
                std::accumulate(jump_groups.begin(), jump_groups.end(), set_t<T>{}, [](auto result, auto i) {
                        return result ^ T::jump_group(i);
                }) == T::squares
        );
}

BOOST_AUTO_TEST_SUITE_END()
