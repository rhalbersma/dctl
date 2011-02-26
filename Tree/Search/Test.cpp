#include "Test.h"
#include "Root.h"
#include "../../Position/Position.h"
#include "../../Geometry/Board.h"
#include "../../Variants/Rules.h"
#include "../../IO/BoardIO.h"
#include "../../IO/FEN.h"
#include <utility>

// http://www.xs4all.nl/~mdgsoft/draughts/stats/index.html
void Tree::Search::Test::International(void)
{
        //Position<Geometry::International> i10 = Position<Geometry::International>::initial();
        //Root::analyze<Variant::International>(i10, 21);

        typedef std::pair<std::string, size_t> DB_unittest;
        int value;       
        
        DB_unittest DB_winGWD[] = {
                DB_unittest("W:W28,31,32,35,36,37,38,39,40,42,43,45,47:B3,7,8,11,12,13,15,19,20,21,23,26,29."   , 21),
                DB_unittest("W:W27,28,32,35,38,40,43,44,45,47,48:B3,8,9,10,11,14,17,19,23,25,29."               , 21),
                DB_unittest("W:W16,21,22,23,26,27,28,31,32,38,43,44,47,50:B2,4,6,7,8,9,10,12,13,24,25,30,35."   , 21),
                DB_unittest("W:W21,24,29,31,34,36,37,42,48,49:B1,7,9,10,12,13,17,22,26,28."                     , 21),
                DB_unittest("W:W15,22,27,31,36,40,44,50:B1,2,4,7,19,23,35."                                     , 21),
                DB_unittest("W:W15,20,21,24,27,34,35,40,48,49:B4,8,9,10,11,12,13,18,33,38."                     , 21),
                DB_unittest("W:W11,16,23,28,29,32,33,34,37,38,40,50:B2,3,4,6,7,9,12,13,15,18,36,45."            , 21),
                DB_unittest("W:W12,19,23,24,26,32,40,41,42,44,47,48,50:B3,8,11,13,17,21,28,30,33,35,36,39,43."  , 21),
                DB_unittest("W:W26,27,28,32,34,37,38,40,42,44,49,50:B1,7,9,10,11,13,16,19,20,23,24,25."         , 21),
                DB_unittest("W:W26,28,30,34,37,39,41,42,43,44,45:B7,8,12,14,17,19,21,24,25,35,36."              , 21),
                DB_unittest("W:W18,23,27,33,34,35,36,39,46,47,50:B1,6,7,8,10,17,20,24,25,37,45."                , 21),
                DB_unittest("W:W26,32,33,35,38,41,43,49,50:B4,8,14,17,18,24,25,37."                             , 21),
                DB_unittest("W:W21,27,34,42,46,49:B8,11,16,18,23,24."                                           , 21)
        };

        for (size_t i = 0; i < 13; ++i) {
                value = Root::analyze<Variant::International>(read_position_string<FEN_tag>()(DB_winGWD[i].first), DB_winGWD[i].second);
                assert(value == Value::win(DB_winGWD[i].second));
        }
/*
        DB_unittest DB_win11[] = {
                DB_unittest("W:W33:B2."  , 17),         // 1010
                DB_unittest("W:WK42:B22.",  7),         // 0110
                DB_unittest("W:W10:BK46.",  3),         // 1001
                DB_unittest("W:WK10:BK5.",  3)          // 0101
        };
                
        for (size_t i = 0; i < 4; ++i) {
                value = Root::analyze<Variant::International>(read_position_string<FEN_tag>()(DB_win11[i].first), DB_win11[i].second);
                assert(value == Value::win(DB_win11[i].second));
        }

        DB_unittest DB_win21[] = {
                DB_unittest("W:W40,44:B3."   , 23),     // 2010
                DB_unittest("W:WK2,11:B45."  , 11),     // 1110
                DB_unittest("W:WK7,K30:B45." ,  9),     // 0210
                DB_unittest("W:W6,7:BK45."   ,  7),     // 2001
                DB_unittest("W:WK2,6:BK50."  ,  9),     // 1101
                DB_unittest("W:WK1,K7:BK50." ,  7),     // 0201
                DB_unittest("W:W21:B3,4."    , 33),     // 1020
                DB_unittest("W:W23:B5,6"     , 29),     // David & Goliath    
                DB_unittest("W:WK38:B4,18."  , 27),     // 0120
                DB_unittest("W:W18:B5,K23."  , 17),     // 1011
                DB_unittest("W:WK26:B23,K42.",  9),     // 0111
                DB_unittest("W:W14:BK10,K46.",  3),     // 1002
                DB_unittest("W:WK10:BK5,K23.",  3)      // 0102
        };

        for (size_t i = 0; i < 13; ++i) {
                value = Root::analyze<Variant::International>(read_position_string<FEN_tag>()(DB_win21[i].first), DB_win21[i].second);
                assert(value == Value::win(DB_win21[i].second));
        }
        
        DB_unittest DB_win22[] = {
                DB_unittest("W:W33,46:B4,5."     , 39), // 2020
                DB_unittest("W:W8,K50:B3,32."    , 27), // 1120
                DB_unittest("W:WK1,K23:B4,38."   , 25), // 0220
                DB_unittest("W:W17,35:B3,K21."   , 23), // 2011
                DB_unittest("W:WK1,12:B16,K50."  , 19), // 1111
                DB_unittest("W:WK1,K16:BK17,26." , 19), // 0211
                DB_unittest("W:W6,12:BK7,K45."   ,  7), // 2002
                DB_unittest("W:W6,K22:BK17,K50." ,  9), // 1102
                DB_unittest("W:WK6,K22:BK17,K50.",  9)  // 0202
        };

        for (size_t i = 0; i < 9; ++i) {
                value = Root::analyze<Variant::International>(read_position_string<FEN_tag>()(DB_win22[i].first), DB_win22[i].second);
                assert(value == Value::win(DB_win22[i].second));
        }
        
        DB_unittest DB_win31[] = {
                DB_unittest("W:W12,13,16:B30."    , 23),        // 3010
                DB_unittest("W:WK4,16,18:B34."    , 21),        // 2110
                DB_unittest("W:WK8,11,K13:B44."   , 19),        // 1210
                DB_unittest("W:WK8,K20,K37:B43."  , 17),        // 0310
                DB_unittest("W:W6,10,49:BK40."    , 13),        // 3001
                DB_unittest("W:W6,K49,50:BK16."   , 29),        // 2101
                DB_unittest("W:WK40,K45,50:BK11." , 31),        // 1201
                DB_unittest("W:WK6,K22,K27:BK49." , 15),        // 0301
                DB_unittest("W:W6:B2,3,4."        , 43),        // 1030
                DB_unittest("W:WK1:B2,3,4."       , 43),        // 0130
                DB_unittest("W:W6:B3,4,K11."      , 33),        // 1021
                DB_unittest("W:WK14:B1,K10,12."   , 29),        // 0121
                DB_unittest("W:W8:B5,K13,K23."    , 17),        // 1012
                DB_unittest("W:WK1:BK18,24,K44."  ,  9),        // 0112
                DB_unittest("W:W23:BK10,K19,K46." ,  3),        // 1003
                DB_unittest("W:WK49:BK23,K43,K46.",  3)         // 0103       
        };
        
        for (size_t i = 0; i < 16; ++i) {
                value = Root::analyze<Variant::International>(read_position_string<FEN_tag>()(DB_win31[i].first), DB_win31[i].second);
                assert(value == Value::win(DB_win31[i].second));
        }
*/
}

// http://www.xs4all.nl/~mdgsoft/draughts/stats/kill-index.html        
void Tree::Search::Test::Killer(void)
{
        typedef std::pair<std::string, size_t> DB_unittest;                
        int value;

        DB_unittest DB_win11[] = {
                DB_unittest("W:W31:B5."   , 17),        // 1010
                DB_unittest("W:WK49:B29." ,  7),        // 0110
                DB_unittest("W:W10:BK46." ,  3),        // 1001
                DB_unittest("W:WK10:BK46.",  3)         // 0101
        };

        for (size_t i = 0; i < 4; ++i) {
                value = Root::analyze<Variant::Killer>(read_position_string<FEN_tag>()(DB_win11[i].first), DB_win11[i].second);
                assert(value == Value::win(DB_win11[i].second));
        }

        DB_unittest DB_win21[] = {
                DB_unittest("W:W41,46:B24."  , 63),     // 2010
                DB_unittest("B:W23:B5,6"     , 57),     // David & Goliath with black to move    
                DB_unittest("W:WK37,46:B41." , 47),     // 1110
                DB_unittest("W:WK7,K30:B45." ,  9),     // 0210
                DB_unittest("W:W21,46:BK47." , 57),     // 2001
                DB_unittest("W:WK1,46:BK47." , 49),     // 1101
                DB_unittest("W:WK5,K14:BK33.", 15),     // 0201
                DB_unittest("W:W21:B3,4."    , 33),     // 1020
                DB_unittest("W:W23:B5,6"     , 29),     // David & Goliath    
                DB_unittest("W:WK38:B3,18."  , 27),     // 0120
                DB_unittest("W:W18:B5,K23."  , 17),     // 1011
                DB_unittest("W:WK16:B24,K43.",  9),     // 0111
                DB_unittest("W:W14:BK10,K46.",  3),     // 1002
                DB_unittest("W:WK10:BK5,K41.",  3)      // 0102
        };       
        
        for (size_t i = 0; i < 14; ++i) {
                value = Root::analyze<Variant::Killer>(read_position_string<FEN_tag>()(DB_win21[i].first), DB_win21[i].second);
                assert(value == Value::win(DB_win21[i].second));
        }

        DB_unittest DB_win22[] = {
                DB_unittest("W:W31,49:B9,14."    , 77), // 2020
                DB_unittest("W:WK4,36:B7,38."    , 67), // 1120
                DB_unittest("W:WK29,K47:B39,44." , 27), // 0220
                DB_unittest("W:W37,42:B20,K41."  , 65), // 2011
                DB_unittest("W:W31,K38:B7,K36."  , 67), // 1111
                DB_unittest("W:WK17,K39:BK21,26.", 31), // 0211
                DB_unittest("W:W6,46:BK11,K31."  , 57), // 2002
                DB_unittest("W:WK37,46:BK41,K43.", 53), // 1102
                DB_unittest("W:WK5,K23:BK19,K50.", 17)  // 0202
        };

        for (size_t i = 0; i < 9; ++i) {
                value = Root::analyze<Variant::Killer>(read_position_string<FEN_tag>()(DB_win22[i].first), DB_win22[i].second);
                assert(value == Value::win(DB_win22[i].second));
        }
        
        DB_unittest DB_win31[] = {
                DB_unittest("W:W37,43,46:B44."    , 69),        // 3010
                DB_unittest("W:WK37,41,46:B36."   , 47),        // 2110
                DB_unittest("W:WK14,K20,33:B44."  , 15),        // 1210
                DB_unittest("W:WK7,K9,K40:B41."   ,  9),        // 0310
                DB_unittest("W:W46,49,50:BK36."   , 73),        // 3001
                DB_unittest("W:WK19,24,46:BK15."  , 51),        // 2101
                DB_unittest("W:WK11,K19,43:BK2."  , 43),        // 1201
                DB_unittest("W:WK12,K37,K43:BK48.", 13),        // 0301
                DB_unittest("W:W6:B3,4,5."        , 43),        // 1030
                DB_unittest("W:WK1:B2,4,5."       , 43),        // 0130
                DB_unittest("W:W7:B3,4,K11."      , 33),        // 1021
                DB_unittest("W:WK16:B4,13,K32."   , 29),        // 0121
                DB_unittest("W:W6:B5,K11,K22."    , 17),        // 1012
                DB_unittest("W:WK1:B24,K29,K44."  ,  9),        // 0112
                DB_unittest("W:W23:BK10,K19,K46." ,  3),        // 1003
                DB_unittest("W:WK48:BK10,K42,K46.",  3)         // 0103       
        };
        
        for (size_t i = 0; i < 16; ++i) {
                value = Root::analyze<Variant::Killer>(read_position_string<FEN_tag>()(DB_win31[i].first), DB_win31[i].second);
                assert(value == Value::win(DB_win31[i].second));
        }

        //Position<Geometry::International> Walinga(read_position_string<FEN_tag>()("W:WK46,28:BK43"));
        //Root::analyze<FrisianRules>(Walinga, 39);
}

/*
void Tree::Search::Test::ChessVariants(void)
{
        // The alternative game rules thread on the FMJD forum http://laatste.info/bb3/viewtopic.php?f=53&t=2822
        Position<Geometry::Chess> i8;
        std::cout << write_square_layout<Geometry::Chess>()() << std::endl;

        Root::analyze<BrazilianRules>(i8, 19);    // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        Root::analyze<PoolRules>(i8, 19);         // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
	Root::analyze<RussianRules>(i8, 19);      // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::analyze<EnglishRules>(i8, 19);      // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
	Root::analyze<CzechRules>(i8, 19);

        Position<Geometry::Roman> r8;
        std::cout << write_square_layout<Geometry::Roman>()() << std::endl;

	Root::analyze<SpanishRules>(r8, 19);
        Root::analyze<ItalianRules>(r8, 19);

        // Addional Italian test positions from email conversations with Ed Gilbert
        Position<Geometry::Roman> ITA_Ed_Gilbert_2 = read_position_string<FEN_tag, Geometry::Roman>()("W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3");
        Position<Geometry::Roman> ITA_Ed_Gilbert_3 = read_position_string<FEN_tag, Geometry::Roman>()("B:W30,21,22,17,20,K6:B25,28,9,5,1,3");
        Root::analyze<ItalianRules>(ITA_Ed_Gilbert_2, 19);
        Root::analyze<ItalianRules>(ITA_Ed_Gilbert_3, 19);

        Position<Geometry::Thai> t8;
        Root::analyze<ThaiRules>(t8, 19);		// Thai initial position with 2 rows of men
        Root::analyze<ThaiRules>(i8, 19);		// Usual initial position with 3 rows of men

        Root::analyze<FrisianRules>(i8, 19);

        Position<MiniBoard> i6;
        Root::analyze<Variant::International>(i6, 31);
}
*/