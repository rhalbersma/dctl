#include "Test.h"
#include "Root.h"
#include "../../Position/Position.h"
#include "../../Geometry/Board.h"
#include "../../Variants/Rules.h"
#include "../../IO/BoardIO.h"
#include "../../IO/FEN.h"
#include <iomanip>
#include <iostream>

// The original perft thread on the FMJD forum http://laatste.info/bb3/viewtopic.php?f=53&t=2308
void Tree::Walk::Test::International(void)
{
        std::cout << write_square_layout<Geometry::International>()() << std::endl;
        std::cout << write_bit_layout<Geometry::International>()() << std::endl;

        Position<Geometry::International> i10 = Position<Geometry::International>::initial();
        Root::perft<Variants::International>(i10, 11);

        Position<Geometry::International> random178(read_position_string<FEN_tag, Geometry::International>()("B:BK17,K24:W6,9,10,11,20,21,22,23,30,K31,33,37,41,42,43,44,46"));
        Root::perft<Variants::International>(random178, 9);

        Position<Geometry::International> Woldouby(read_position_string<FEN_tag, Geometry::International>()("W:B12,13,14,16,18,19,21,23,24,26:W25,27,28,30,32,33,34,35,37,38"));
        Root::perft<Variants::International>(Woldouby, 15);
}

// The rectangular board thread on the FMJD forum: http://laatste.info/bb3/viewtopic.php?f=53&t=3014
void Tree::Walk::Test::Rectangular(void)
{
        std::cout << write_square_layout<Geometry::Spantsireti>()() << std::endl;
        std::cout << write_bit_layout<Geometry::Spantsireti>()() << std::endl;

        Position<Geometry::Spantsireti> iS;
        Root::perft<Variants::International>(iS, 9);

        std::cout << write_square_layout<Geometry::Ktar11>()() << std::endl;
        std::cout << write_bit_layout<Geometry::Ktar11>()() << std::endl;
        
        Position<Geometry::Ktar11> iK11;
        Root::perft<Variants::International>(iK11, 9);
                
        std::cout << write_square_layout<Geometry::Ktar12>()() << std::endl;
        std::cout << write_bit_layout<Geometry::Ktar12>()() << std::endl;
        
        Position<Geometry::Ktar12> iK12;
        Root::perft<Variants::International>(iK12, 9);
}

// The alternative game rules thread on the FMJD forum: http://laatste.info/bb3/viewtopic.php?f=53&t=2822
void Tree::Walk::Test::ChessVariants(void)
{
        Position<Geometry::Chess> i8;
        std::cout << write_square_layout<Geometry::Chess>()() << std::endl;
        
        Root::perft<Variants::Brazilian>(i8, 13); // http://shashki.com/PNphpBB2-viewtopic-t-627-start-78.html
        Root::perft<Variants::Pool>(i8, 13);      // http://shashki.com/PNphpBB2-viewtopic-t-627-start-57.html
        Root::perft<Variants::Russian>(i8, 14);   // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::perft<Variants::English>(i8, 14);   // http://shashki.com/PNphpBB2-viewtopic-t-627-start-126.html
        Root::perft<Variants::Czech>(i8, 13);

        Position<Geometry::Roman> r8;
        std::cout << write_square_layout<Geometry::Roman>()() << std::endl;

        Root::perft<Variants::Spanish>(r8, 13);
        Root::perft<Variants::Italian>(r8, 13);

        // Addional Italian test positions from email conversation with Ed Gilbert
        Position<Geometry::Roman> ITA_Ed_Gilbert_2 = read_position_string<FEN_tag, Geometry::Roman>()("W:W30,26,27,22,23,24,17,18,20:B14,15,16,9,11,5,6,1,3");
        Position<Geometry::Roman> ITA_Ed_Gilbert_3 = read_position_string<FEN_tag, Geometry::Roman>()("B:W30,21,22,17,20,K6:B25,28,9,5,1,3");
        Root::perft<Variants::Italian>(ITA_Ed_Gilbert_2, 16);
        Root::perft<Variants::Italian>(ITA_Ed_Gilbert_3, 12);
        
        Position<Geometry::Thai> t8;
        Root::perft<Variants::Thai>(t8, 11);      // Thai initial position with 2 rows of men
        Root::perft<Variants::Thai>(i8, 13);      // Usual initial position with 3 rows of men
}

// Test positions from the official Italian rules: http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
void Tree::Walk::Test::ItalianRuleBook(void)
{
        Position<Geometry::Roman> ITA_empty = read_position_string<FEN_tag, Geometry::Roman>()("");     // Art. 2.1
        typedef Geometry::Roman ITA_notation;                                      	                // Art. 2.4
        Position<Geometry::Roman> ITA_initial;						                // Art. 2.6

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

        std::cout << write_position_layout<FEN_tag>()(ITA_empty) << std::endl;
        write_square_layout<ITA_notation>()();
        std::cout << write_position_layout<FEN_tag>()(ITA_initial) << std::endl;
        for (size_t i = 0; i < 9; ++i)
                Root::divide<Variants::Italian>(read_position_string<FEN_tag, Geometry::Roman>()(ITA_rules[i]), 1);
}
