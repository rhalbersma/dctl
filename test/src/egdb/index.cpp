//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/egdb/index.hpp>          // subdatabase
#include <dctl/core.hpp>                // state, international, board
#include <dctl/util/type_traits.hpp>    // position_t
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END
#include <vector>                       // vector

using namespace dctl::egdb;
using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(Index)

BOOST_AUTO_TEST_CASE(NumeralIsInverseOfPosition)
{
        using state_type = basic_state<international, basic_board<micro>>;

        using subdivision_type = subdivision<position_t<state_type>>;
        auto const subdivisions = std::vector<subdivision_type>
        {
                { 4, 0, 0, 0 },
                { 4, 0, 4, 0 },
                { 0, 4, 0, 0 },
                { 0, 4, 0, 4 },
                { 2, 2, 0, 0 },
                { 2, 2, 2, 0 },
                { 2, 2, 0, 2 },
                { 2, 2, 2, 2 },
                { 4, 0, 2, 0 },
                { 0, 4, 0, 2 },
                { 2, 2, 1, 1 }
        };

        for (auto const& subdb : subdivisions) {
                for (auto i = 0LL; i < subdb.size(); ++i) {
                        if (auto const p = subdb.position(i); p) {
                                BOOST_CHECK_EQUAL(subdb.index(p.value()), i);
                        }
                }
        }
}

BOOST_AUTO_TEST_SUITE_END()
