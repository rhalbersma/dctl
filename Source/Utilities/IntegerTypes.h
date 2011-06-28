#pragma once
#include <cstddef>
#include <stdint.h>

typedef uint8_t PieceCount;
typedef uint8_t MoveCount;
typedef uint8_t PlyCount;
typedef uint64_t NodeCount;
typedef uint64_t HashIndex;
typedef uint64_t BitBoard;

template<typename> struct log2_sizeof;
template<> struct log2_sizeof<uint8_t>  { enum { value = 3 }; };
template<> struct log2_sizeof<uint16_t> { enum { value = 4 }; };
template<> struct log2_sizeof<uint32_t> { enum { value = 5 }; };
template<> struct log2_sizeof<uint64_t> { enum { value = 6 }; };

template<size_t> struct exp2_typeof;
template<> struct exp2_typeof<3>        { typedef uint8_t  TYPE; };
template<> struct exp2_typeof<4>        { typedef uint16_t TYPE; };
template<> struct exp2_typeof<5>        { typedef uint32_t TYPE; };
template<> struct exp2_typeof<6>        { typedef uint64_t TYPE; };
