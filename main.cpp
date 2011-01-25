#include "Position/Position.h"
#include "Search/SearchTest.h"
#include "Perft/PerftTest.h"
#include <iostream>
#include <iomanip>

#include "CheckerBoard/CheckerBoardAPI.h"
#include "DamExchange/Test.h"
#include "DamExchange/Client.h"

int main(void)
{     
        DamExchangeProtocol::Test::Mesander_examples();

        std::cout << sizeof(Position<InternationalBoard>) << std::endl;

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
