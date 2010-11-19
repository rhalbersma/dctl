#pragma once
#include "IntegerTypes.h"

class Bit
{
public:
	// unary predicates
	template<typename T> static bool is_zero(T);			// 0 bits set to 1
        template<typename T> static bool is_single(T);			// 1 bit set to 1
        template<typename T> static bool is_double(T);			// 2 bits set to 1
        template<typename T> static bool is_multiple(T);		// 2 or more bits set to 1

        // binary predicates
        template<typename T> static bool is_within(T, T);
        template<typename T> static bool is_exclusive(T, T);

        template<typename T> static T get_lowest(T);			// least significant 1-bit
        template<typename T> static T except_lowest(T);			// most significant 1-bits
        template<typename T> static void clear_lowest(T&);		// clear the least significant 1-bit
        template<typename T> static size_t scan_forward(T);             // index of the least significant 1-bit
        template<typename T> static size_t index(T);			// index of a set 1-bit
        template<typename T> static size_t count(T);			// number of set 1-bits
        template<bool, typename T> static T flood_fill(T, T, size_t);

private:
        template<typename T> static size_t index_DeBruijn(T);
        template<typename T> static size_t count_Kernighan(T);
        template<bool, typename T> static T dumb_fill(T, T, size_t);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Bit.hpp"
