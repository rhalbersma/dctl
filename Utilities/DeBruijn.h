#pragma once
#include <cstddef>
#include <cstdint>

template<size_t N>
struct DeBruijn
{
        typedef uint64_t T;                             // perform all bitwise operations in 64-bit mode
        static const T SEQUENCE;                        // lexicographically least De Bruijn sequence
        static T generate_sequence(void);

        static const size_t POW2N = 1 << N;             // 2^N
        static const size_t SHIFT = POW2N - N;          // 2^N - N
        static const size_t MASK = POW2N - 1;           // mask of N least significant bits
        static const size_t TABLE[POW2N];               // table with the 2^N unique N-bit prefixes
        static void generate_table(void);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "DeBruijn.hpp"
