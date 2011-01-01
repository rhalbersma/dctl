#include <iomanip>
#include <iostream>

// create the lexicographic least De Bruijn sequence
template<size_t N>
typename DeBruijn<N>::T DeBruijn<N>::generate_Ford_sequence(void)
{
        T sequence = 0;

        // highest N bits are always zero; only have to try the remaining bits (SHIFT == 2^N - N)
        for (int i = SHIFT - 1; i >= 0; --i) {
                T new_prefix = (sequence >> i) & PREFIX_MASK;

                // greedy algorithm: set the least significant bit of the next prefix equal to one
                new_prefix ^= T(1);

                // test if the new prefix matches any of the previous prefixes
                bool prefer_one = true;
                for (int j = SHIFT - 1; j > i; --j) {
                        T old_prefix = (sequence >> j) & PREFIX_MASK;
                        if (old_prefix == new_prefix) {
                                prefer_one = false;
                                break;
                        }
                }
 
                // if the new prefix did not occur before, add a one bit to the sequence
                if (prefer_one)
                        sequence ^= T(1) << i;
        }

        return sequence;
}

template<size_t N>
void DeBruijn<N>::generate_prefix_table(void)
{
        const T Ford_sequence = DeBruijn<N>::generate_Ford_sequence();

        size_t prefix_table[POW2N];
        for (size_t i = 0; i < POW2N; ++i)
                prefix_table[((Ford_sequence << i) >> SHIFT) & PREFIX_MASK] = i;

        std::cout << "const DeBruijn<" << N << ">::T DeBruijn<" << N << ">::FORD_SEQUENCE = ";
        std::cout << "0x" << std::hex << std::setw(POW2N / 4) << std::setfill('0');
        std::cout << Ford_sequence << ";" << std::endl << std::endl;
        
        std::cout << "const size_t DeBruijn<" << N << ">::PREFIX_TABLE[] = {" << std::endl;
        for (size_t i = 0; i < POW2N; ++i) {
                if (i % 8 == 0)
                        for (size_t j = 0; j < 8; ++j)
                                std::cout << " ";
                std::cout << std::dec << std::setw(2) << std::setfill(' ') << prefix_table[i];
                if (i != POW2N - 1)
                        std::cout << ",";
                if (i % 8 == 7 || i == POW2N - 1)
                        std::cout << std::endl;
                else 
                        std::cout << " ";
        }
        std::cout << "};" << std::endl << std::endl;
}
