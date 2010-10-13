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
