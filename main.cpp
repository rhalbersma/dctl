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

#include "Board/Board.h"
#include "Board/BoardPredicates.h"

int main(void)
{
        std::cout << ChessBoard::MIN_BITS << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 0>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 1>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 2>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 3>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 4>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 5>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 6>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 7>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 8>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 9>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 10>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 11>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 12>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 13>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 14>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 15>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 16>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 17>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 18>::VALUE) << std::endl;

        std::cout << (IS_GHOST_BIT<ChessBoard, 40>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 41>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 42>::VALUE) << std::endl;
        std::cout << (IS_GHOST_BIT<ChessBoard, 43>::VALUE) << std::endl;

        //PerftSuite::International();
	//PerftSuite::ChessBoardVariants();
	//PerftSuite::ItalianRuleBook();

	std::cout << "End of program." << std::endl;
        for (;;);
}

/*
TODO:
-implement small endgame tests (3 vs 1, 2 vs 1)
*/
