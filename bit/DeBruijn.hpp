#pragma once
#include <iomanip>                      // std::setfill, std::setw
#include <iostream>                     // std::cout
#include <typeinfo>                     // typeid
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace bit {

template
<
        typename T = uint64_t
>
class DeBruijn
{
public:
        static int index(T b)
        {
                b *= SEQUENCE;
                b >>= SHIFT;
                return TABLE[b];
        }

        // compute the lexicographically least De Bruijn sequence
        static T generate_sequence()
        {
                T sequence(0);

                // highest N bits are always zero; only have to try the remaining bits (SHIFT == 2^N - N)
                for (auto i = SHIFT - 1; i >= 0; --i) {
                        // cast the intermediate expression back to <T> to suppress promotion of small integer types
                        auto new_prefix = static_cast<T>(sequence << (SHIFT - i)) >> SHIFT;

                        // greedy algorithm: set the least significant bit of the next prefix equal to one
                        new_prefix ^= T(1);

                        // test if the new prefix matches any of the previous prefixes
                        bool prefer_one = true;
                        for (auto j = SHIFT - 1; j > i; --j) {
                                // cast the intermediate expression back to <T> to suppress promotion of small integer types
                                auto old_prefix = static_cast<T>(sequence << (SHIFT - j)) >> SHIFT;
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
        
        // compute the prefix table and print to stdout
        static void generate_table()
        {
                const auto sequence = generate_sequence();

                int table[NUM_BITS];
                for (auto i = 0; i < NUM_BITS; ++i) {
                        T b = T(1) << i;
                        b *= sequence;
                        b >>= SHIFT;
                        table[b] = i;
                }

                std::cout << "template<>\n";
                std::cout << "const " << typeid(T).name() << " DeBruijn<" << typeid(T).name() << ">::SEQUENCE = ";
                std::cout << "0x" << std::hex << std::setw(NUM_BITS / 4) << std::setfill('0');
                std::cout << static_cast<uint64_t>(sequence) << ";\n\n";
        
                std::cout << "template<>\n";
                std::cout << "const int DeBruijn<" << typeid(T).name() << ">::TABLE[] = {\n";
                for (auto i = 0; i < NUM_BITS; ++i) {
                        if (i % 8 == 0)
                                for (auto j = 0; j < 8; ++j)
                                        std::cout << " ";
                        std::cout << std::dec << std::setw(2) << std::setfill(' ') << table[i];
                        if (i != NUM_BITS - 1)
                                std::cout << ",";
                        if (i % 8 == 7 || i == NUM_BITS - 1)
                                std::cout << "\n";
                        else 
                                std::cout << " ";
                }
                std::cout << "};" << "\n\n";
        }                   

private:
        BOOST_STATIC_CONSTANT(auto, N = log2_sizeof<T>::value); // log base-2 of number of bits
        BOOST_STATIC_CONSTANT(auto, NUM_BITS = 1 << N);         // 2^N
        BOOST_STATIC_CONSTANT(auto, SHIFT = NUM_BITS - N);      // 2^N - N
        static const T SEQUENCE;                                // lexicographically least De Bruijn sequence
        static const int TABLE[NUM_BITS];                       // table with the 2^N unique N-bit prefixes
};

// The remainder of this file has been copied from the output of the following main program:

/*
#include "../src/bit/DeBruijn.h"

int main()
{
        using namespace dctl::bit; 
        DeBruijn<uint8_t >::generate_table();
        DeBruijn<uint16_t>::generate_table();
        DeBruijn<uint32_t>::generate_table();
        DeBruijn<uint64_t>::generate_table();
}
*/
/*
template<typename Dummy>
const unsigned char DeBruijn<unsigned char, Dummy>::SEQUENCE = 0x1d;

template<typename Dummy>
const int DeBruijn<unsigned char, Dummy>::TABLE[] = {
         0,  1,  6,  2,  7,  5,  4,  3
};

// explicit instantation
template class DeBruijn<unsigned char>;

template<typename Dummy>
const unsigned short DeBruijn<unsigned short, Dummy>::SEQUENCE = 0x0f65;

template<typename Dummy>
const int DeBruijn<unsigned short, Dummy>::TABLE[] = {
         0,  1, 11,  2, 14, 12,  8,  3,
        15, 10, 13,  7,  9,  6,  5,  4
};

// explicit instantation
template class DeBruijn<unsigned short>;

template<typename Dummy>
const unsigned int DeBruijn<unsigned int, Dummy>::SEQUENCE = 0x07dcd629;

template<unsigned int, typename Dummy>
const int DeBruijn<unsigned int>::TABLE[] = {
         0,  1, 23,  2, 29, 24, 14,  3,
        30, 27, 25, 18, 20, 15, 10,  4,
        31, 22, 28, 13, 26, 17, 19,  9,
        21, 12, 16,  8, 11,  7,  6,  5
};

// explicit instantation
template class DeBruijn<unsigned int>;

template<typename Dummy>
const unsigned __int64 DeBruijn<unsigned __int64, Dummy>::SEQUENCE = 0x03f79d71b4cb0a89;

template<unsigned __int64, typename Dummy>
const int DeBruijn<unsigned __int64, Dummy>::TABLE[] = {
         0,  1, 48,  2, 57, 49, 28,  3,
        61, 58, 50, 42, 38, 29, 17,  4,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 18, 12,  5,
        63, 47, 56, 27, 60, 41, 37, 16,
        54, 35, 52, 21, 44, 32, 23, 11,
        46, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19,  9, 13,  8,  7,  6
};

// explicit instantation
template class DeBruijn<unsigned __int64>;
*/
}       // namespace bit
}       // namespace dctl
