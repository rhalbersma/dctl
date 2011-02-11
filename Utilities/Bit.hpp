#include "InlineOptions.h"
#include "DeBruijn.h"
#include "Shift.h"
#include <cassert>

namespace {

//+----------------------------------------------------------------------------+
//|      Leiserson, Prokop and Randall, 1998                                   |
//|      http://supertech.csail.mit.edu/papers/debruijn.pdf                    |
//+----------------------------------------------------------------------------+

template<typename> struct log2_sizeof;
template<> struct log2_sizeof<uint8_t>           { enum { value = 3 }; };
template<> struct log2_sizeof<uint16_t>          { enum { value = 4 }; };
template<> struct log2_sizeof<uint32_t>          { enum { value = 5 }; };
template<> struct log2_sizeof<uint64_t>          { enum { value = 6 }; };

template<typename T>
size_t index_DeBruijn(T b)
{
        static const size_t N = log2_sizeof<T>::value;
        b *= static_cast<T>(DeBruijn<N>::SEQUENCE);
        b >>= DeBruijn<N>::SHIFT;
        return DeBruijn<N>::TABLE[b];
}

//+----------------------------------------------------------------------------+
//|      Kernighan & Ritchie, The C programming language, 2nd Ed.              |
//|      https://chessprogramming.wikispaces.com/Population+Count              |
//+----------------------------------------------------------------------------+

template<typename T> FORCE_INLINE
size_t count_Kernighan(T b)
{
        size_t count = 0;
        for (; b; Bit::clear_lowest(b))
                ++count;
        return count;
}

//+-------------------------------------------------------------------------------------+
//|       Chess Programming Wiki, "Fill Loop" algorithm                                 |
//|       http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop  |
//+-------------------------------------------------------------------------------------+

template<bool Sign, typename T> FORCE_INLINE
T fill_loop(T generator, T propagator, size_t dir)
{
        T flood = 0;
        while (generator) {
                flood |= generator;
                generator = Shift<Sign>()(generator, dir) & propagator;
        }
        return flood;
}

}       // namespace

namespace Bit {

template<typename T>
bool is_zero(T b)
{
        return b == 0;
}

template<typename T>
bool is_single(T b)
{
        return !(is_zero(b) || is_multiple(b));
}

template<typename T>
bool is_double(T b)
{
        return is_single(except_lowest(b));
}

template<typename T>
bool is_multiple(T b)
{
        return !is_zero(except_lowest(b));
}

template<typename T>
bool is_within(T a, T b)
{
        return is_zero(a & ~b);
}

template<typename T>
bool is_exclusive(T a, T b)
{
        return is_zero(a & b);
}

template<typename T>
T get_lowest(T b)
{
        return b & (0 - b);
}

template<typename T>
T except_lowest(T b)
{
        return b & (b - 1);
}

template<typename T>
size_t scan_forward(T b)
{
        return index(get_lowest(b));
}

template<typename T>
size_t index(T b)
{
	assert(is_single(b));
        return index_DeBruijn(b);
}

template<typename T>
size_t count(T b)
{
        return count_Kernighan(b);
}

template<bool Sign, typename T>
T flood_fill(T generator, T propagator, size_t dir)
{
        return fill_loop<Sign>(generator, propagator, dir);
}

template<typename T>
void clear_lowest(T& b)
{
        b &= b - 1;
}

}       // namespace Bit
