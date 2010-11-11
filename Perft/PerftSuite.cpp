#include "PerftSuite.h"
#include "Perft.h"
#include "Divide.h"
#include "../Position/Position.h"
#include "../Board/Board.h"
#include "../Rules/Rules.h"
#include "../IO/BoardIO.h"
#include "../IO/PositionIO.h"
#include <iostream>

void PerftSuite::International(void)
{
	// The original perft thread on the FMJD forum http://laatste.info/bb3/viewtopic.php?f=53&t=2308
        Position<> i10;
        Position<> random178(read_position_string<FEN>()("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"));
        Position<> Woldouby(read_position_string<FEN>()("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"));
        std::cout << write_square_layout<InternationalBoard>()() << std::endl;
        std::cout << write_bit_layout<InternationalBoard>()() << std::endl;

        Perft::root<InternationalRules>(i10, 11);
        Perft::root<InternationalRules>(random178, 9);
        Perft::root<InternationalRules>(Woldouby, 15);
}

void PerftSuite::Rectangular(void)
{
	// The original perft thread on the FMJD forum http://laatste.info/bb3/viewtopic.php?f=53&t=2308
        Position<SpantsiretiBoard> iS;
        std::cout << write_square_layout<SpantsiretiBoard>()() << std::endl;
        std::cout << write_bit_layout<SpantsiretiBoard>()() << std::endl;
        Perft::root<InternationalRules>(iS, 9);

        Position<Ktar11Board> iK11;
        std::cout << write_square_layout<Ktar11Board>()() << std::endl;
        std::cout << write_bit_layout<Ktar11Board>()() << std::endl;
        Perft::root<InternationalRules>(iK11, 9);
                
        Position<Ktar12Board> iK12;
        std::cout << write_square_layout<Ktar12Board>()() << std::endl;
        std::cout << write_bit_layout<Ktar12Board>()() << std::endl;
        Perft::root<InternationalRules>(iK12, 9);
}

void PerftSuite::ChessBoardVariants(void)
{
        // The alternative game rules thread on the FMJD forum http://laatste.info/bb3/viewtopic.php?f=53&t=2822
        Position<ChessBoard> i8;
        std::cout << write_square_layout<ChessBoard>()() << std::endl;
        
        Perft::root<BrazilianRules>(i8, 13);    // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        Perft::root<PoolRules>(i8, 13);         // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
        Perft::root<RussianRules>(i8, 14);      // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Perft::root<EnglishRules>(i8, 14);      // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Perft::root<CzechRules>(i8, 13);

        Position<RomanBoard> r8;
        std::cout << write_square_layout<RomanBoard>()() << std::endl;

        Perft::root<SpanishRules>(r8, 13);
        Perft::root<ItalianRules>(r8, 13);

        // Addional Italian test positions from email conversation with Ed Gilbert
        Position<RomanBoard> ITA_Ed_Gilbert_2 = read_position_string<FEN, RomanBoard>()("W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3");
        Position<RomanBoard> ITA_Ed_Gilbert_3 = read_position_string<FEN, RomanBoard>()("B:W30,21,22,17,20,K6:B25,28,9,5,1,3");
        Perft::root<ItalianRules>(ITA_Ed_Gilbert_2, 16);
        Perft::root<ItalianRules>(ITA_Ed_Gilbert_3, 12);

        Position<ThaiBoard> t8;
        Perft::root<ThaiRules>(t8, 11);		// Thai initial position with 2 rows of men
        Perft::root<ThaiRules>(i8, 13);		// Usual initial position with 3 rows of men
}

void PerftSuite::ItalianRuleBook(void)
{
        // test positions from the official Italian rules
        // http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
        Position<RomanBoard> ITA_empty = read_position_string<FEN, RomanBoard>()("");	// Art. 2.1
        typedef RomanBoard ITA_notation;                                      	        // Art. 2.4
        Position<RomanBoard> ITA_initial;						// Art. 2.6

        Position<RomanBoard> ITA_test;
        std::string ITA_rules[] = {
                "W:W30:B27",                    // Art. 5.6
                "W:W31:B12,20,28",              // Art. 5.7
                "W:WK22:B18,19,27",             // Art. 5.8
                "W:WK27:B5,6,13,15,21,23",      // Art. 5.9
                "W:WK21,31:B10,18,27",          // Art. 6.6
                "W:WK23,31:B19,26,27",          // Art. 6.7
                "W:WK23:B19,K28",               // Art. 6.8
                "W:WK30:B10,12,18,K20,K26,27",  // Art. 6.9
                "W:WK31:B18,20,K27,K28"         // Art. 6.10
        };

        std::cout << write_position_layout<FEN>()(ITA_empty) << std::endl;
        write_square_layout<ITA_notation>()();
        std::cout << write_position_layout<FEN>()(ITA_initial) << std::endl;
        for (size_t i = 0; i < 9; ++i) {
                ITA_test = read_position_string<FEN, RomanBoard>()(ITA_rules[i]);
                Divide::root<ItalianRules>(ITA_test, 1);
        }
}
