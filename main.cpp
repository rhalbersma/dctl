#include "Search/SearchTest.h"
#include "Perft/PerftTest.h"
#include "Utilities/IntegerTypes.h"
#include <iostream>
#include <iomanip>

int main(void)
{
        PerftTest::International();
        //SearchTest::Killer();
        //PerftTest::Rectangular();
	//PerftTest::ChessBoardVariants();
	//PerftTest::ItalianRuleBook();

	std::cout << "End of program." << std::endl;
        //for (;;);
}

/*
TODO:
-implement missing square layouts
-implement test harness for hash function
-generate DeBruijn constants
*/
