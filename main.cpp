#include "Search/SearchTest.h"
#include "Perft/PerftTest.h"
#include "Utilities/IntegerTypes.h"
#include <iostream>
#include <iomanip>

#include "CheckerBoard/CheckerBoardAPI.h"
#include "DamExchange/Layer2/DXP_Test.h"
#include "Utilities/DeBruijn.h"

int main(void)
{
     
        DXP_Test::Mesander_examples();

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
-implement DXP Layer 1 (Boost sockets)
-implement algebraic notation
-implement missing square layouts
-implement test harness for hash function
-search...
-eval...
*/
