#include <iomanip>
#include <iostream>

#include "Tree/Search/Test.h"
#include "Tree/Walk/Test.h"

#include "DamExchange/Layer1/Test.h"
#include "DamExchange/Layer2/Test.h"

int main(void)
{     
        //DamExchange::Layer2::Test::Mesander_examples();
        //DamExchange::Layer1::Test::connection();

        Tree::Walk::Test::International();
        //Tree::Walk::Test::Killer();
        //Tree::Walk::Test::Rectangular();
        //Tree::Walk::Test::ChessBoardVariants();
        //Tree::Walk::Test::ItalianRuleBook();
        
        std::cout << "End of program." << std::endl;
        for (;;);
}

/*
TODO:
-implement algebraic notation
-implement missing square layouts
-implement test harness for hash function
-search...
-eval...
*/
