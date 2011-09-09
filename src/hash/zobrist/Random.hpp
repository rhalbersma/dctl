#include "../../bit/Bit.h"

namespace dctl {
namespace hash {
namespace zobrist {

template<typename Index>
Index Random<Index>::xor_rand(BitBoard b, const Index* random)
{
        Index hash = 0;
        for (; b; bit::clear_first(b))
                hash ^= random[bit::find_first(b)];
        return hash;
}

template<typename Index>
Index Random<Index>::xor_rand(PlyCount s, const Index* random)
{
        return s? random[s - 1] : 0;
}

template<typename Index>
Index Random<Index>::xor_rand(bool to_move, Index random)
{
        return to_move? random : 0;
}

}       // namespace zobrist
}       // namespace hash
}       // namespace dctl

/*
uint64_t rand64()
{
        uint64_t key = rand();

        for (auto i = 0; i < 4; ++i) {
                key <<= 15;
                key ^= rand();
        }

        return  key;
}

void generate_Zobrist()
{
	for (auto i = 0; i < 64; ++i) {
		std::cout << "0x" << std::hex << std::right << std::setfill('0') << std::setw(16);
		std::cout << rand64();
		if (i != 63)
			std::cout << ",";
		if (3 == (i % 4))
			std::cout << "\n";
		else
			std::cout << " ";
	}
	std::cout << "\n";
}
*/
