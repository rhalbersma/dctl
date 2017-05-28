#include <algo/search/fixture.hpp>
#include <dctl/core/board.hpp>
#include <dctl/core/rules/international.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

namespace dctl {
namespace aima {
namespace search {

BOOST_AUTO_TEST_SUITE(SearchEGDBinternational)

using F = Fixture< state<rules::international, board::international>, DefaultObjective >;

BOOST_FIXTURE_TEST_CASE(DavidGoliath, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W23:B5,6", 29 },   // David & Goliath
        };
        run(tests);
}

// Michel Grimminkc's statistics on longest wins in the international endgame databases
// http://www.xs4all.nl/~mdgsoft/draughts/stats/index.html

BOOST_FIXTURE_TEST_CASE(Longestl11, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W33:B2"  , 17 },   // 1010
                { "W:WK42:B22",  7 },   // 0110
                { "W:W10:BK46",  3 },   // 1001
                { "W:WK10:BK5",  3 },   // 0101
        };
        run(tests);
}

BOOST_FIXTURE_TEST_CASE(Longest21, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W40,44:B3"   , 23 },       // 2010
                { "W:WK2,11:B45"  , 11 },       // 1110
                { "W:WK7,K30:B45" ,  9 },       // 0210
                { "W:W6,7:BK45"   ,  7 },       // 2001
                { "W:WK2,6:BK50"  ,  9 },       // 1101
                { "W:WK1,K7:BK50" ,  7 },       // 0201
                { "W:W21:B3,4"    , 33 },       // 1020
                { "W:WK38:B4,18"  , 27 },       // 0120
                { "W:W18:B5,K23"  , 17 },       // 1011
                { "W:WK26:B23,K42",  9 },       // 0111
                { "W:W14:BK10,K46",  3 },       // 1002
                { "W:WK10:BK5,K23",  3 },       // 0102
        };
        run(tests);
}

BOOST_FIXTURE_TEST_CASE(Longest22, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W33,46:B4,5"     , 39 },   // 2020
                { "W:W8,K50:B3,32"    , 27 },   // 1120
                { "W:WK1,K23:B4,38"   , 25 },   // 0220
                { "W:W17,35:B3,K21"   , 23 },   // 2011
                { "W:WK1,12:B16,K50"  , 19 },   // 1111
                { "W:WK1,K16:BK17,26" , 19 },   // 0211
                { "W:W6,12:BK7,K45"   ,  7 },   // 2002
                { "W:W6,K22:BK17,K50" ,  9 },   // 1102
                { "W:WK6,K22:BK17,K50",  9 },   // 0202
        };
        run(tests);
}

BOOST_FIXTURE_TEST_CASE(Longest31, F)
{
        auto const tests = std::vector<Test>
        {
                { "W:W12,13,16:B30"    , 23 },  // 3010
                { "W:WK4,16,18:B34"    , 21 },  // 2110
                { "W:WK8,11,K13:B44"   , 19 },  // 1210
                { "W:WK8,K20,K37:B43"  , 17 },  // 0310
                { "W:W6,10,49:BK40"    , 13 },  // 3001
                { "W:W6,K49,50:BK16"   , 29 },  // 2101
                { "W:WK40,K45,50:BK11" , 31 },  // 1201
                { "W:WK6,K22,K27:BK49" , 15 },  // 0301
                { "W:W6:B2,3,4"        , 43 },  // 1030
                { "W:WK1:B2,3,4"       , 43 },  // 0130
                { "W:W6:B3,4,K11"      , 33 },  // 1021
                { "W:WK14:B1,K10,12"   , 29 },  // 0121
                { "W:W8:B5,K13,K23"    , 17 },  // 1012
                { "W:WK1:BK18,24,K44"  ,  9 },  // 0112
                { "W:W23:BK10,K19,K46" ,  3 },  // 1003
                { "W:WK49:BK23,K43,K46",  3 },  // 0103
        };
        run(tests);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace aima
}       // namespace dctl
