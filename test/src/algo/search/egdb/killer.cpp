//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algo/search/fixture.hpp>
#include <dctl/core.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

using namespace dctl::algo;
using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(SearchEGDBKiller)

using F = search::Fixture<basic_state<killer<international>, basic_board<international>>, search::DefaultObjective >;

BOOST_FIXTURE_TEST_CASE(DavidGoliath, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W23:B5,6", 29 },   // David & Goliath
                { "B:W23:B5,6", 57 },   // David & Goliath with black to move
        };
        run(tests);
}

// Michel Grimminkc's statistics on longest wins in the killer endgame databases
// http://www.xs4all.nl/~mdgsoft/draughts/stats/kill-index.html

BOOST_FIXTURE_TEST_CASE(Longest11, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W31:B5"   , 17 },  // 1010
                { "W:WK49:B29" ,  7 },  // 0110
                { "W:W10:BK46" ,  3 },  // 1001
                { "W:WK10:BK46",  3 },  // 0101
        };
        run(tests);
}

BOOST_FIXTURE_TEST_CASE(Longest21, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W41,46:B24"  , 63 },       // 2010
                { "W:WK37,46:B41" , 47 },       // 1110
                { "W:WK7,K30:B45" ,  9 },       // 0210
                { "W:W21,46:BK47" , 57 },       // 2001
                { "W:WK1,46:BK47" , 49 },       // 1101
                { "W:WK5,K14:BK33", 15 },       // 0201
                { "W:W21:B3,4"    , 33 },       // 1020
                { "W:WK38:B3,18"  , 27 },       // 0120
                { "W:W18:B5,K23"  , 17 },       // 1011
                { "W:WK16:B24,K43",  9 },       // 0111
                { "W:W14:BK10,K46",  3 },       // 1002
                { "W:WK10:BK5,K41",  3 },       // 0102
        };
        run(tests);
}

BOOST_FIXTURE_TEST_CASE(Longest22, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W31,49:B9,14"    , 77 },   // 2020
                { "W:WK4,36:B7,38"    , 67 },   // 1120
                { "W:WK29,K47:B39,44" , 27 },   // 0220
                { "W:W37,42:B20,K41"  , 65 },   // 2011
                { "W:W31,K38:B7,K36"  , 67 },   // 1111
                { "W:WK17,K39:BK21,26", 31 },   // 0211
                { "W:W6,46:BK11,K31"  , 57 },   // 2002
                { "W:WK37,46:BK41,K43", 53 },   // 1102
                { "W:WK5,K23:BK19,K50", 17 },   // 0202
        };
        run(tests);
}

BOOST_FIXTURE_TEST_CASE(Longest31, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W37,43,46:B44"    , 69 },  // 3010
                { "W:WK37,41,46:B36"   , 47 },  // 2110
                { "W:WK14,K20,33:B44"  , 15 },  // 1210
                { "W:WK7,K9,K40:B41"   ,  9 },  // 0310
                { "W:W46,49,50:BK36"   , 73 },  // 3001
                { "W:WK19,24,46:BK15"  , 51 },  // 2101
                { "W:WK11,K19,43:BK2"  , 43 },  // 1201
                { "W:WK12,K37,K43:BK48", 13 },  // 0301
                { "W:W6:B3,4,5"        , 43 },  // 1030
                { "W:WK1:B2,4,5"       , 43 },  // 0130
                { "W:W7:B3,4,K11"      , 33 },  // 1021
                { "W:WK16:B4,13,K32"   , 29 },  // 0121
                { "W:W6:B5,K11,K22"    , 17 },  // 1012
                { "W:WK1:B24,K29,K44"  ,  9 },  // 0112
                { "W:W23:BK10,K19,K46" ,  3 },  // 1003
                { "W:WK48:BK10,K42,K46",  3 },  // 0103
        };
        run(tests);
}

BOOST_AUTO_TEST_SUITE_END()
