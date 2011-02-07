#include "TreeSearch/Test.h"
#include "TreeWalk/Test.h"
#include <iostream>
#include <iomanip>

#include "CheckerBoard/CheckerBoardAPI.h"
#include "DamExchange/Test.h"
#include "DamExchange/Client.h"

int main(void)
{     
        DamExchangeProtocol::Test::Mesander_examples();

        DamExchangeProtocol::Client my_connection("127.0.0.1", "27531");
        std::string line;
        while (std::getline(std::cin, line))
        {                
                my_connection.write(DamExchangeProtocol::StringMessage(line));
        }
        my_connection.close();

        /*
        TreeWalk::Test::International();
        //TreeWalk::Test::Killer();
        TreeWalk::Test::Rectangular();
        TreeWalk::Test::ChessBoardVariants();
        TreeWalk::Test::ItalianRuleBook();
        */
        std::cout << "End of program." << std::endl;
        for (;;);

}

/*
TODO:
-split Capture into Capture + MoveList, change Move into MoveList
-test DXP Layer 1 (using Boost sockets)
-implement algebraic notation
-implement missing square layouts
-implement test harness for hash function
-search...
-eval...
*/
