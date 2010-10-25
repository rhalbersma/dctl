#include "Search/SearchSuite.h"
#include "Perft/PerftSuite.h"
#include "Utilities/IntegerTypes.h"
#include <iostream>
#include <iomanip>

#include "Board/Board.h"
#include "IO/BoardIO.h"

int main(void)
{
        PerftSuite::International();
	PerftSuite::ChessBoardVariants();
	//PerftSuite::ItalianRuleBook();

	std::cout << "End of program." << std::endl;
        for (;;);
}

/*
TODO:
-decouple internal/external board layout
-implement small endgame tests (3 vs 1, 2 vs 1)
-implement test harness for hash function
*/
