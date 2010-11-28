#include "SearchSuite.h"
#include "Search.h"
#include "../Position/Position.h"
#include "../Board/Board.h"
#include "../Rules/Rules.h"
#include "../IO/BoardIO.h"
#include "../IO/PositionIO.h"
#include <utility>

void SearchSuite::International(void)
{
        typedef std::pair<std::string, size_t> DB_unittest;
        
        // http://www.xs4all.nl/~mdgsoft/draughts/stats/index.html
        
        DB_unittest DB_win11[] = {
                DB_unittest("W:W33:B2."      , 17),     // 1010
                DB_unittest("W:WK42:B22."    ,  7),     // 0110
                DB_unittest("W:W10:BK46."    ,  3),     // 1001
                DB_unittest("W:WK10:BK5."    ,  3)      // 0101
        };
                
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

        int value;

        for (size_t i = 0; i < 4; ++i) {
                value = Search::root<InternationalRules>(read_position_string<FEN>()(DB_win11[i].first), DB_win11[i].second);
                assert(value == SearchValue::win(DB_win11[i].second));
        }
        
        for (size_t i = 0; i < 13; ++i) {
                value = Search::root<InternationalRules>(read_position_string<FEN>()(DB_win21[i].first), DB_win21[i].second);
                assert(value == SearchValue::win(DB_win21[i].second));
        }
}

void SearchSuite::Killer(void)
{
        typedef std::pair<std::string, size_t> DB_unittest;

        // http://www.xs4all.nl/~mdgsoft/draughts/stats/kill-index.html

        DB_unittest DB_win11[] = {
                DB_unittest("W:W31:B5."      , 17),     // 1010
                DB_unittest("W:WK49:B29."    ,  7),     // 0110
                DB_unittest("W:W10:BK46."    ,  3),     // 1001
                DB_unittest("W:WK10:BK46."   ,  3)      // 0101
        };
                
        DB_unittest DB_win21[] = {
                DB_unittest("W:W41,46:B24."  , 63),     // 2010
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

        int value;

        for (size_t i = 0; i < 4; ++i) {
                value = Search::root<KillerRules>(read_position_string<FEN>()(DB_win11[i].first), DB_win11[i].second);
                assert(value == SearchValue::win(DB_win11[i].second));
        }
        
        for (size_t i = 0; i < 13; ++i) {
                value = Search::root<KillerRules>(read_position_string<FEN>()(DB_win21[i].first), DB_win21[i].second);
                assert(value == SearchValue::win(DB_win21[i].second));
        }

        //Position<> Walinga(read_position_string<FEN>()("W:WK46,28:BK43"));
        //Search::root<FrisianRules>(Walinga, 39);
}

/*
void SearchSuite::ChessBoardVariants(void)
{
        // The alternative game rules thread_pos on the FMJD forum http://laatste.info/bb3/viewtopic.php?f=53&t=2822
        Position<ChessBoard> i8;
        std::cout << write_square_layout<ChessBoard>()() << std::endl;

        Search::root<BrazilianRules>(i8, 19);    // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        Search::root<PoolRules>(i8, 19);         // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
	Search::root<RussianRules>(i8, 19);      // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Search::root<EnglishRules>(i8, 19);      // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
	Search::root<CzechRules>(i8, 19);

        Position<RomanBoard> r8;
        std::cout << write_square_layout<RomanBoard>()() << std::endl;

	Search::root<SpanishRules>(r8, 19);
        Search::root<ItalianRules>(r8, 19);

        // Addional Italian test positions from email conversations with Ed Gilbert
        Position<RomanBoard> ITA_Ed_Gilbert_2 = read_position_string<FEN, RomanBoard>()("W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3");
        Position<RomanBoard> ITA_Ed_Gilbert_3 = read_position_string<FEN, RomanBoard>()("B:W30,21,22,17,20,K6:B25,28,9,5,1,3");
        Search::root<ItalianRules>(ITA_Ed_Gilbert_2, 19);
        Search::root<ItalianRules>(ITA_Ed_Gilbert_3, 19);

        Position<ThaiBoard> t8;
        Search::root<ThaiRules>(t8, 19);		// Thai initial position with 2 rows of men
        Search::root<ThaiRules>(i8, 19);		// Usual initial position with 3 rows of men

        Search::root<FrisianRules>(i8, 19);

        Position<MiniBoard> i6;
        Search::root<InternationalRules>(i6, 31);
}
*/