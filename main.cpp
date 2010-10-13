#include "Search/SearchSuite.h"
#include "Perft/PerftSuite.h"
#include "Utilities/IntegerTypes.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

uint64_t rand64(void)
{
        uint64_t key = rand();

        for (size_t i = 0; i < 4; ++i) {
                key <<= 15;
                key ^= rand();
        }

        return  key;
}

void generate_Zobrist(void)
{
	for (size_t i = 0; i < 64; ++i) {
		std::cout << "0x" << std::hex << std::right << std::setfill('0') << std::setw(16);
		std::cout << rand64();
		if (i != 63)
			std::cout << ",";
		if (3 == (i % 4))
			std::cout << std::endl;
		else
			std::cout << " ";
	}
	std::cout << std::endl;
}

int main(void)
{
        PerftSuite::International();
	PerftSuite::ChessBoardVariants();
	PerftSuite::ItalianRuleBook();

	std::cout << "End of program." << std::endl;
        for (;;);
}

/*
TODO:
-implement small endgame tests (3 vs 1, 2 vs 1)
*/
