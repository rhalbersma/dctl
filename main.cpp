#include "Search/SearchTest.h"
#include "Perft/PerftTest.h"
#include "Utilities/IntegerTypes.h"
#include <iostream>
#include <iomanip>

#include "CheckerBoard/CheckerBoardAPI.h"
#include "DamExchange/DamExchangeAPI.h"
#include "Utilities/DeBruijn.h"

int main(void)
{
        /*
        DeBruijn<1>::generate_prefix_table();
        DeBruijn<2>::generate_prefix_table();
        DeBruijn<3>::generate_prefix_table();
        DeBruijn<4>::generate_prefix_table();
        DeBruijn<5>::generate_prefix_table();
        DeBruijn<6>::generate_prefix_table();
        */

        PerftTest::International();
        //SearchTest::Killer();
        //PerftTest::Rectangular();
	//PerftTest::ChessBoardVariants();
	//PerftTest::ItalianRuleBook();

	std::cout << "End of program." << std::endl;
        for (;;);
}

/*
TODO:
-implement algebraic notation
-implement missing square layouts
-implement test harness for hash function
*/
