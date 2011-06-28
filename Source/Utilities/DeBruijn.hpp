#include <iomanip>
#include <iostream>

// create the lexicographically least De Bruijn sequence
template<typename T>
T DeBruijn<T>::generate_sequence(void)
{
        T sequence(0);

        // highest N bits are always zero; only have to try the remaining bits (SHIFT == 2^N - N)
        for (int i = SHIFT - 1; i >= 0; --i) {
                T new_prefix = (sequence >> i) & MASK;

                // greedy algorithm: set the least significant bit of the next prefix equal to one
                new_prefix ^= T(1);

                // test if the new prefix matches any of the previous prefixes
                bool prefer_one = true;
                for (int j = SHIFT - 1; j > i; --j) {
                        T old_prefix = (sequence >> j) & MASK;
                        if (new_prefix == old_prefix) {
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

template<typename T>
void DeBruijn<T>::generate_table(void)
{
        const T sequence = generate_sequence();

        size_t table[POW2N];
        for (auto i = 0; i < POW2N; ++i) {
                T b = T(1) << i;
                b *= sequence;
                b >>= SHIFT;
                table[b] = i;
        }

        std::cout << "template<>" << std::endl;
        std::cout << "const DeBruijn<" << N << ">::T DeBruijn<" << N << ">::SEQUENCE = ";
        std::cout << "0x" << std::hex << std::setw(POW2N / 4) << std::setfill('0');
        std::cout << static_cast<uint64_t>(sequence) << ";" << std::endl << std::endl;
        
        std::cout << "template<>" << std::endl;
        std::cout << "const size_t DeBruijn<" << N << ">::TABLE[] = {" << std::endl;
        for (auto i = 0; i < POW2N; ++i) {
                if (i % 8 == 0)
                        for (auto j = 0; j < 8; ++j)
                                std::cout << " ";
                std::cout << std::dec << std::setw(2) << std::setfill(' ') << table[i];
                if (i != POW2N - 1)
                        std::cout << ",";
                if (i % 8 == 7 || i == POW2N - 1)
                        std::cout << std::endl;
                else 
                        std::cout << " ";
        }
        std::cout << "};" << std::endl << std::endl;
}
