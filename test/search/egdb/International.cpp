#include <boost/test/unit_test.hpp>
#include "../Fixture.hpp"
#include "../../test_config.hpp"
#include "../../../src/board/Types.hpp"
#include "../../../src/rules/Types.hpp"

#if SEARCH_EGDB_INTERNATIONAL == 1

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(TestInternational)

BOOST_FIXTURE_TEST_CASE(InternationalDavidGoliath, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W23:B5,6", 29)     // David & Goliath
        };
        run<rules::International, board::International>(tests);
}

// Michel Grimminkc's statistics on longest wins in the International endgame databases
// http://www.xs4all.nl/~mdgsoft/draughts/stats/index.html

BOOST_FIXTURE_TEST_CASE(International11, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W33:B2"  , 17),   // 1010
                FEN_depth("W:WK42:B22",  7),   // 0110
                FEN_depth("W:W10:BK46",  3),   // 1001
                FEN_depth("W:WK10:BK5",  3)    // 0101
        };
        run<rules::International, board::International>(tests);
}

BOOST_FIXTURE_TEST_CASE(International21, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W40,44:B3"   , 23),        // 2010
                FEN_depth("W:WK2,11:B45"  , 11),        // 1110
                FEN_depth("W:WK7,K30:B45" ,  9),        // 0210
                FEN_depth("W:W6,7:BK45"   ,  7),        // 2001
                FEN_depth("W:WK2,6:BK50"  ,  9),        // 1101
                FEN_depth("W:WK1,K7:BK50" ,  7),        // 0201
                FEN_depth("W:W21:B3,4"    , 33),        // 1020
                FEN_depth("W:WK38:B4,18"  , 27),        // 0120
                FEN_depth("W:W18:B5,K23"  , 17),        // 1011
                FEN_depth("W:WK26:B23,K42",  9),        // 0111
                FEN_depth("W:W14:BK10,K46",  3),        // 1002
                FEN_depth("W:WK10:BK5,K23",  3)         // 0102
        };
        run<rules::International, board::International>(tests);
}

BOOST_FIXTURE_TEST_CASE(International22, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W33,46:B4,5"     , 39),    // 2020
                FEN_depth("W:W8,K50:B3,32"    , 27),    // 1120
                FEN_depth("W:WK1,K23:B4,38"   , 25),    // 0220
                FEN_depth("W:W17,35:B3,K21"   , 23),    // 2011
                FEN_depth("W:WK1,12:B16,K50"  , 19),    // 1111
                FEN_depth("W:WK1,K16:BK17,26" , 19),    // 0211
                FEN_depth("W:W6,12:BK7,K45"   ,  7),    // 2002
                FEN_depth("W:W6,K22:BK17,K50" ,  9),    // 1102
                FEN_depth("W:WK6,K22:BK17,K50",  9)     // 0202
        };
        run<rules::International, board::International>(tests);
}

BOOST_FIXTURE_TEST_CASE(International31, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W12,13,16:B30"    , 23),   // 3010
                FEN_depth("W:WK4,16,18:B34"    , 21),   // 2110
                FEN_depth("W:WK8,11,K13:B44"   , 19),   // 1210
                FEN_depth("W:WK8,K20,K37:B43"  , 17),   // 0310
                FEN_depth("W:W6,10,49:BK40"    , 13),   // 3001
                FEN_depth("W:W6,K49,50:BK16"   , 29),   // 2101
                FEN_depth("W:WK40,K45,50:BK11" , 31),   // 1201
                FEN_depth("W:WK6,K22,K27:BK49" , 15),   // 0301
                FEN_depth("W:W6:B2,3,4"        , 43),   // 1030
                FEN_depth("W:WK1:B2,3,4"       , 43),   // 0130
                FEN_depth("W:W6:B3,4,K11"      , 33),   // 1021
                FEN_depth("W:WK14:B1,K10,12"   , 29),   // 0121
                FEN_depth("W:W8:B5,K13,K23"    , 17),   // 1012
                FEN_depth("W:WK1:BK18,24,K44"  ,  9),   // 0112
                FEN_depth("W:W23:BK10,K19,K46" ,  3),   // 1003
                FEN_depth("W:WK49:BK23,K43,K46",  3)    // 0103
        };
        run<rules::International, board::International>(tests);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace dctl

#endif
