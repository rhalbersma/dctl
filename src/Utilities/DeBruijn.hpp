#include <iomanip>
#include <iostream>

namespace bit {

// create the lexicographically least De Bruijn sequence
template<typename T>
T DeBruijn<T>::generate_sequence(void)
{
        T sequence(0);

        // highest N bits are always zero; only have to try the remaining bits (SHIFT == 2^N - N)
        for (int i = SHIFT - 1; i >= 0; --i) {
                // cast the intermediate expression back to <T> to suppress promotion of small integer types
                T new_prefix = static_cast<T>(sequence << (SHIFT - i)) >> SHIFT;

                // greedy algorithm: set the least significant bit of the next prefix equal to one
                new_prefix ^= T(1);

                // test if the new prefix matches any of the previous prefixes
                bool prefer_one = true;
                for (int j = SHIFT - 1; j > i; --j) {
                        // cast the intermediate expression back to <T> to suppress promotion of small integer types
                        T old_prefix = static_cast<T>(sequence << (SHIFT - j)) >> SHIFT;
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

        size_t table[NUM_BITS];
        for (auto i = 0; i < NUM_BITS; ++i) {
                T b = T(1) << i;
                b *= sequence;
                b >>= SHIFT;
                table[b] = i;
        }

        std::cout << "template<>" << std::endl;
        std::cout << "const " << typeid(T).name() << " bit::DeBruijn<" << typeid(T).name() << ">::SEQUENCE = ";
        std::cout << "0x" << std::hex << std::setw(NUM_BITS / 4) << std::setfill('0');
        std::cout << static_cast<uint64_t>(sequence) << ";" << std::endl << std::endl;
        
        std::cout << "template<>" << std::endl;
        std::cout << "const size_t bit::DeBruijn<" << typeid(T).name() << ">::TABLE[] = {" << std::endl;
        for (auto i = 0; i < NUM_BITS; ++i) {
                if (i % 8 == 0)
                        for (auto j = 0; j < 8; ++j)
                                std::cout << " ";
                std::cout << std::dec << std::setw(2) << std::setfill(' ') << table[i];
                if (i != NUM_BITS - 1)
                        std::cout << ",";
                if (i % 8 == 7 || i == NUM_BITS - 1)
                        std::cout << std::endl;
                else 
                        std::cout << " ";
        }
        std::cout << "};" << std::endl << std::endl;
}

}       // namespace bit
