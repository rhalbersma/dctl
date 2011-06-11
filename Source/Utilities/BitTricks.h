#pragma once
#include "IntegerTypes.h"

namespace bit {

template<typename> struct log2_sizeof;
template<> struct log2_sizeof<uint8_t>           { enum { value = 3 }; };
template<> struct log2_sizeof<uint16_t>          { enum { value = 4 }; };
template<> struct log2_sizeof<uint32_t>          { enum { value = 5 }; };
template<> struct log2_sizeof<uint64_t>          { enum { value = 6 }; };

// unary views
template<typename T> size_t index_DeBruijn(T);          // index of a set 1-bit
template<typename T> size_t count_Kernighan(T);         // number of set 1-bits

// binary views
template<bool, typename T> T fill_loop(T, T, size_t);   // direction-wise flood-fill T1 over T2

}       // namespace bit

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "bitTricks.hpp"
