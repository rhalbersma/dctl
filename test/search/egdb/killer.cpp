#include <test/config.hpp>

#if SEARCH_EGDB_KILLER == 1

#include <boost/test/unit_test.hpp>
#include <test/search/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules/types.hpp>

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(TestKiller)

BOOST_FIXTURE_TEST_CASE(killerDavidGoliath, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W23:B5,6", 29),    // David & Goliath
                FEN_depth("B:W23:B5,6", 57)     // David & Goliath with black to move
        };
        run<rules::Killer, board::International>(tests);
}

// Michel Grimminkc's statistics on longest wins in the Killer endgame databases
// http://www.xs4all.nl/~mdgsoft/draughts/stats/kill-index.html

BOOST_FIXTURE_TEST_CASE(killer11, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W31:B5"   , 17),   // 1010
                FEN_depth("W:WK49:B29" ,  7),   // 0110
                FEN_depth("W:W10:BK46" ,  3),   // 1001
                FEN_depth("W:WK10:BK46",  3)    // 0101
        };
        run<rules::Killer, board::International>(tests);
}

BOOST_FIXTURE_TEST_CASE(killer21, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W41,46:B24"  , 63),        // 2010
                FEN_depth("W:WK37,46:B41" , 47),        // 1110
                FEN_depth("W:WK7,K30:B45" ,  9),        // 0210
                FEN_depth("W:W21,46:BK47" , 57),        // 2001
                FEN_depth("W:WK1,46:BK47" , 49),        // 1101
                FEN_depth("W:WK5,K14:BK33", 15),        // 0201
                FEN_depth("W:W21:B3,4"    , 33),        // 1020
                FEN_depth("W:WK38:B3,18"  , 27),        // 0120
                FEN_depth("W:W18:B5,K23"  , 17),        // 1011
                FEN_depth("W:WK16:B24,K43",  9),        // 0111
                FEN_depth("W:W14:BK10,K46",  3),        // 1002
                FEN_depth("W:WK10:BK5,K41",  3)         // 0102
        };
        run<rules::Killer, board::International>(tests);
}

BOOST_FIXTURE_TEST_CASE(killer22, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W31,49:B9,14"    , 77),    // 2020
                FEN_depth("W:WK4,36:B7,38"    , 67),    // 1120
                FEN_depth("W:WK29,K47:B39,44" , 27),    // 0220
                FEN_depth("W:W37,42:B20,K41"  , 65),    // 2011
                FEN_depth("W:W31,K38:B7,K36"  , 67),    // 1111
                FEN_depth("W:WK17,K39:BK21,26", 31),    // 0211
                FEN_depth("W:W6,46:BK11,K31"  , 57),    // 2002
                FEN_depth("W:WK37,46:BK41,K43", 53),    // 1102
                FEN_depth("W:WK5,K23:BK19,K50", 17)     // 0202
        };
        run<rules::Killer, board::International>(tests);
}

BOOST_FIXTURE_TEST_CASE(killer31, Fixture<DefaultObjective>)
{
        FEN_depth tests[] = {
                FEN_depth("W:W37,43,46:B44"    , 69),   // 3010
                FEN_depth("W:WK37,41,46:B36"   , 47),   // 2110
                FEN_depth("W:WK14,K20,33:B44"  , 15),   // 1210
                FEN_depth("W:WK7,K9,K40:B41"   ,  9),   // 0310
                FEN_depth("W:W46,49,50:BK36"   , 73),   // 3001
                FEN_depth("W:WK19,24,46:BK15"  , 51),   // 2101
                FEN_depth("W:WK11,K19,43:BK2"  , 43),   // 1201
                FEN_depth("W:WK12,K37,K43:BK48", 13),   // 0301
                FEN_depth("W:W6:B3,4,5"        , 43),   // 1030
                FEN_depth("W:WK1:B2,4,5"       , 43),   // 0130
                FEN_depth("W:W7:B3,4,K11"      , 33),   // 1021
                FEN_depth("W:WK16:B4,13,K32"   , 29),   // 0121
                FEN_depth("W:W6:B5,K11,K22"    , 17),   // 1012
                FEN_depth("W:WK1:B24,K29,K44"  ,  9),   // 0112
                FEN_depth("W:W23:BK10,K19,K46" ,  3),   // 1003
                FEN_depth("W:WK48:BK10,K42,K46",  3)    // 0103
        };
        run<rules::Killer, board::International>(tests);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace dctl

#endif
