#include "Search/SearchSuite.h"
#include "Perft/PerftSuite.h"
#include "Utilities/IntegerTypes.h"
#include <iostream>
#include <iomanip>

#include "Board/Board.h"
#include "IO/BoardIO.h"
#include "Board/GridPredicates.h"
#include "Board/Grid.h"

int main(void)
{
        std::cout << write_bit_layout< SpantsiretiBoard >()() << std::endl;
        std::cout << write_bit_layout< Ktar11Board      >()() << std::endl;
        std::cout << write_bit_layout< Ktar12Board      >()() << std::endl;

        std::cout << InternationalBoard::ANGLE << " " << InternationalBoard::A_PRIME << std::endl;
        
        for(size_t i = 0; i < 8; ++i) {
                std::cout << InternationalBoard::DIR[i] << " " << InternationalBoard::BASE_DIR[i] << std::endl;
        }

        PerftSuite::International();
	//PerftSuite::ChessBoardVariants();
	//PerftSuite::ItalianRuleBook();

	std::cout << "End of program." << std::endl;
        for (;;);
}

/*
TODO:
-implement rotated ghost bit layouts
-implement missing square layouts
-implement small endgame tests (3 vs 1, 2 vs 1)
-implement test harness for hash function
-generate DeBruijn constants
*/
