//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <core/board/group.hpp>                         // action::is_realized, make_group
#include <core/board/sequence.hpp>                      // micro, mini, checkers, Roman, spantsiretti, international, frisian, ktar<10, 11>,
                                                        // ktar<10, 12>, Compact_10_12, Compact_12_10, rectangular<12, 10>, canadian, srilankan, dumm
#include <dctl/core/board/angle.hpp>                    // _deg, inverse, rotate
#include <dctl/core/board/coordinates.hpp>       // operator==, rotate, to_sco
#include <boost/algorithm/cxx11/all_of.hpp>             // all_of
#include <boost/range/irange.hpp>                       // irange
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_CHECK
#include <type_traits>                                  // common_type
#include <vector>                                       // vector

using namespace dctl::core;
using namespace literals;

BOOST_AUTO_TEST_SUITE(GridCoordinates)

BOOST_AUTO_TEST_CASE_TEMPLATE(GroupActionIsRealizedForAllCyclicGroupsOnAllSquareCoordinates, T, board::BoardSequence)
{
        auto const op = [](auto i, auto j){ return rotate(i, j); };
        auto const inv = [](auto i){ return inverse(i); };

        auto const C1 = make_group(
                { 0_deg },
                op, inv
        );

        auto const C2 = make_group(
                { 0_deg, 180_deg },
                op, inv
        );

        auto const C4 = make_group(
                { 0_deg,  90_deg, 180_deg, 270_deg },
                op, inv
        );

        using CyclicGroup = std::common_type_t<decltype(C1), decltype(C2), decltype(C4)>;

        auto const C_N = std::vector<CyclicGroup>
        {
                C1, C2, C4
        };

        BOOST_CHECK(
                boost::algorithm::all_of(C_N, [=](auto const& g) {
                        return boost::algorithm::all_of(boost::irange(0, T::size()), [&](auto i) {
                                auto const coord = to_sco(i, T::inner_grid);
                                return group::action::is_realized(coord, g);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()
