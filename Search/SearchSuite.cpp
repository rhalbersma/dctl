#include "SearchSuite.h"
#include "Search.h"
#include "../Position/Position.h"
#include "../Board/Board.h"
#include "../Rules/Rules.h"
#include "../IO/BoardIO.h"
#include "../IO/PositionIO.h"

///*
void SearchSuite::International(void)
{
        //Position<> Taille8pc(read_position_string<FEN>()("W:WK34,K46,K49,K50:BK4,26,36,45"));
        //Search::root<InternationalRules>(Taille8pc, 29);

        Position<> DavidGoliath(read_position_string<FEN>()("B:W23:B5,6"));
        Search::root<KillerRules>(DavidGoliath, 57);

        //Position<> Walinga(read_position_string<FEN>()("W:WK46,28:BK43"));
        //Search::root<FrisianRules>(Walinga, 39);

        /*
	// The original perft thread on the FMJD forum http://laatste.info/bb3/viewtopic.php?f=53&t=2308

        Position<> i10;
        Position<> random178(read_position_string<FEN>()("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"));
        Position<> Woldouby(read_position_string<FEN>()("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"));
        std::cout << write_square_layout<InternationalBoard>()() << std::endl;

        Position<> Keller(read_position_string<FEN>()("W:W15,26,31,33,34,35,36,37,38,39,40,42,43,45,46,47,48,49:B3,4,6,7,8,9,11,12,13,14,16,17,18,19,21,22,23,25"));

        Evaluate::print_break_down(Keller);
        Search::root<InternationalRules>(Keller, 11);

        Search::root<InternationalRules>(i10, 11);
        Search::root<InternationalRules>(random178, 19);
        Search::root<InternationalRules>(Woldouby, 25);
        */
}
//*/
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
