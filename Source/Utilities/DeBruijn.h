#pragma once
#include "IntegerTypes.h"

template<typename T>
class DeBruijn
{
private:
        static const size_t N = log2_sizeof<T>::value;  // log base-2 of number of bits
        static const size_t POW2N = 1 << N;             // 2^N
        static const size_t MASK = POW2N - 1;

public:
        static const T SEQUENCE;                        // lexicographically least De Bruijn sequence
        static const size_t SHIFT = POW2N - N;          // 2^N - N
        static const size_t TABLE[POW2N];               // table with the 2^N unique N-bit prefixes

        static T generate_sequence(void);               // compute the lexicographically least De Bruijn sequence
        static void generate_table(void);               // compute the prefix table and print to stdout
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "DeBruijn.hpp"
