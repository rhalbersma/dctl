#pragma once
#include "IntegerTypes.h"

namespace dctl {
namespace bit {

template<typename T>
class DeBruijn
{
public:
        static int index(T);
        static T generate_sequence();                   // compute the lexicographically least De Bruijn sequence
        static void generate_table();                   // compute the prefix table and print to stdout

private:
        static const int N = log2_sizeof<T>::value;     // log base-2 of number of bits
        static const int NUM_BITS = 1 << N;             // 2^N
        static const T SEQUENCE;                        // lexicographically least De Bruijn sequence
        static const int SHIFT = NUM_BITS - N;          // 2^N - N
        static const int TABLE[NUM_BITS];               // table with the 2^N unique N-bit prefixes
};

}       // namespace bit
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "DeBruijn.hpp"
