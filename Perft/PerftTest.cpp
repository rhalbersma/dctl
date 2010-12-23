#include "PerftTest.h"
#include "Perft.h"
#include "Divide.h"
#include "../Position/Position.h"
#include "../Board/Board.h"
#include "../Rules/Rules.h"
#include "../IO/BoardIO.h"
#include "../IO/PositionIO.h"
#include <iostream>

// The original perft thread on the FMJD forum http://laatste.info/bb3/viewtopic.php?f=53&t=2308
void PerftTest::International(void)
{
        std::cout << write_square_layout<InternationalBoard>()() << std::endl;
        std::cout << write_bit_layout<InternationalBoard>()() << std::endl;

        Position<> i10;
        Perft::perft<InternationalRules>(i10, 11);

        Position<> random178(read_position_string<FEN>()("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"));
        Perft::perft<InternationalRules>(random178, 9);

        Position<> Woldouby(read_position_string<FEN>()("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"));
        Perft::perft<InternationalRules>(Woldouby, 15);
}

// The rectangular board thread on the FMJD forum: http://laatste.info/bb3/viewtopic.php?f=53&t=3014
void PerftTest::Rectangular(void)
{
        std::cout << write_square_layout<SpantsiretiBoard>()() << std::endl;
        std::cout << write_bit_layout<SpantsiretiBoard>()() << std::endl;

        Position<SpantsiretiBoard> iS;
        Perft::perft<InternationalRules>(iS, 9);

        std::cout << write_square_layout<Ktar11Board>()() << std::endl;
        std::cout << write_bit_layout<Ktar11Board>()() << std::endl;
        
        Position<Ktar11Board> iK11;
        Perft::perft<InternationalRules>(iK11, 9);
                
        std::cout << write_square_layout<Ktar12Board>()() << std::endl;
        std::cout << write_bit_layout<Ktar12Board>()() << std::endl;
        
        Position<Ktar12Board> iK12;
        Perft::perft<InternationalRules>(iK12, 9);
}

// The alternative game rules thread on the FMJD forum: http://laatste.info/bb3/viewtopic.php?f=53&t=2822
void PerftTest::ChessBoardVariants(void)
{
        Position<ChessBoard> i8;
        std::cout << write_square_layout<ChessBoard>()() << std::endl;
        
        Perft::perft<BrazilianRules>(i8, 13);           // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        Perft::perft<PoolRules>(i8, 13);                // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
        Perft::perft<RussianRules>(i8, 14);             // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Perft::perft<EnglishRules>(i8, 14);             // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Perft::perft<CzechRules>(i8, 13);

        Position<RomanBoard> r8;
        std::cout << write_square_layout<RomanBoard>()() << std::endl;

        Perft::perft<SpanishRules>(r8, 13);
        Perft::perft<ItalianRules>(r8, 13);

        // Addional Italian test positions from email conversation with Ed Gilbert
        Position<RomanBoard> ITA_Ed_Gilbert_2 = read_position_string<FEN, RomanBoard>()("W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3");
        Position<RomanBoard> ITA_Ed_Gilbert_3 = read_position_string<FEN, RomanBoard>()("B:W30,21,22,17,20,K6:B25,28,9,5,1,3");
        Perft::perft<ItalianRules>(ITA_Ed_Gilbert_2, 16);
        Perft::perft<ItalianRules>(ITA_Ed_Gilbert_3, 12);
        
        Position<ThaiBoard> t8;
        Perft::perft<ThaiRules>(t8, 11);                // Thai initial position with 2 rows of men
        Perft::perft<ThaiRules>(i8, 13);		// Usual initial position with 3 rows of men
}


// Test positions from the official Italian rules: http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
void PerftTest::ItalianRuleBook(void)
{
        Position<RomanBoard> ITA_empty = read_position_string<FEN, RomanBoard>()("");	// Art. 2.1
        typedef RomanBoard ITA_notation;                                      	        // Art. 2.4
        Position<RomanBoard> ITA_initial;						// Art. 2.6

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
        for (size_t i = 0; i < 9; ++i)
                Divide::divide<ItalianRules>(read_position_string<FEN, RomanBoard>()(ITA_rules[i]), 1);
}
