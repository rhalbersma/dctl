#include "../Utilities/Bit.h"

template<typename Index>
Index ZobristRandom<Index>::xor_rand(BitBoard b, const Index* random)
{
        Index hash = 0;
        for (; b; Bit::clear_lowest(b))
                hash ^= random[Bit::scan_forward(b)];
        return hash;
}

template<typename Index>
Index ZobristRandom<Index>::xor_rand(PlyCount s, const Index* random)
{
        return s? random[s - 1] : 0;
}

template<typename Index>
Index ZobristRandom<Index>::xor_rand(bool to_move, Index random)
{
        return to_move? random : 0;
}

/*
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
*/