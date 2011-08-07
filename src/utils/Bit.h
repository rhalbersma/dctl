#pragma once
#include <cstddef>

namespace dctl {
namespace bit {

// unary predicates
template<typename T> bool is_zero(T);		        // 0 bits set to 1
template<typename T> bool is_single(T);		        // 1 bit set to 1
template<typename T> bool is_double(T);		        // 2 bits set to 1
template<typename T> bool is_multiple(T);	        // 2 or more bits set to 1

// binary predicates
template<typename T> bool is_within(T, T);              // T1 contained within T2
template<typename T> bool is_exclusive(T, T);           // T1 and T2 mutually exclusive

// unary views
template<typename T> T get_first(T);		        // least significant 1-bit
template<typename T> T except_first(T);	                // most significant 1-bits
template<typename T> size_t find_first(T);              // index of the least significant 1-bit

template<typename T> size_t index(T);		        // index of a set 1-bit
template<typename T> size_t index_DeBruijn(T);          // index of a set 1-bit
template<typename T> size_t index_lookup(T);            // index of a set 1-bit

template<typename T> size_t count(T);		        // number of set 1-bits
template<typename T> size_t count_Kernighan(T);         // number of set 1-bits
template<typename T> size_t count_lookup(T);            // number of set 1-bits

// binary views
template<bool, typename T> T flood_fill(T, T, size_t);  // direction-wise flood-fill T1 over T2
template<bool, typename T> T fill_loop(T, T, size_t);   // direction-wise flood-fill T1 over T2

// unary modifiers
template<typename T> void clear_first(T&);		// clear the least significant 1-bit

}       // namespace bit
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Bit.hpp"
