#include "Search/SearchTest.h"
#include "Perft/PerftTest.h"
#include <iostream>
#include <iomanip>

#include "CheckerBoard/CheckerBoardAPI.h"
#include "DamExchange/DXP_Test.h"

int main(void)
{
     
        DXP_Test::Mesander_examples();

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
-implement DXP Layer 1 (using Boost sockets)
-implement algebraic notation
-implement missing square layouts
-implement test harness for hash function
-search...
-eval...
*/
