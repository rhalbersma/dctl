#pragma once
#include "IntegerTypes.h"

template<typename> struct Log2SizeOf;
template<> struct Log2SizeOf<uint8_t>           { enum { VALUE = 3 }; };
template<> struct Log2SizeOf<uint16_t>          { enum { VALUE = 4 }; };
template<> struct Log2SizeOf<uint32_t>          { enum { VALUE = 5 }; };
template<> struct Log2SizeOf<uint64_t>          { enum { VALUE = 6 }; };

template<size_t> struct DeBruijnType            { typedef uint32_t TYPE; };
template<> struct DeBruijnType<6>               { typedef uint64_t TYPE; };

template<size_t N>
struct DeBruijn
{
        typedef typename DeBruijnType<N>::TYPE T;

        static const T POW2N = 1 << N;                  // 2^N
        static const T PREFIX_MASK = POW2N - T(1);      // mask of N least significant bits
        static const T SHIFT = POW2N - N;               // 2^N - N
        static const T FORD_SEQUENCE;                   // lexicographic least De Bruijn sequence
        static const size_t PREFIX_TABLE[POW2N];

        static T generate_Ford_sequence(void);
        static void generate_prefix_table(void);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "DeBruijn.hpp"
