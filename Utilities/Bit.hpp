#include <cassert>
#include "InlineOptions.h"
#include "DeBruijn.h"
#include "Shift.h"

//+----------------------------------------------------------------------------+
//|      Chess Programming Wiki, general setwise operations                    |
//|      https://chessprogramming.wikispaces.com/General+Setwise+Operations    |
//+----------------------------------------------------------------------------+

template<typename T>
bool Bit::is_zero(T b)
{
        return b == 0;
}

template<typename T>
bool Bit::is_single(T b)
{
        return !(is_zero(b) || is_multiple(b));
}

template<typename T>
bool Bit::is_double(T b)
{
        return is_single(except_lowest(b));
}

template<typename T>
bool Bit::is_multiple(T b)
{
        return !is_zero(except_lowest(b));
}

template<typename T>
bool Bit::is_within(T a, T b)
{
        return is_zero(a & ~b);
}

template<typename T>
bool Bit::is_exclusive(T a, T b)
{
        return is_zero(a & b);
}

template<typename T>
T Bit::get_lowest(T b)
{
        return b & (0 - b);
}

template<typename T>
T Bit::except_lowest(T b)
{
        return b & (b - 1);
}

template<typename T>
void Bit::clear_lowest(T& b)
{
        b &= b - 1;
}

template<typename T>
size_t Bit::scan_forward(T b)
{
        return index(get_lowest(b));
}

template<typename T>
size_t Bit::index(T b)
{
	assert(is_single(b));
        return index_DeBruijn(b);
}

template<typename T>
size_t Bit::count(T b)
{
        return count_Kernighan(b);
}

template<bool Sign, typename T>
T Bit::flood_fill(T generator, T propagator, size_t dir)
{
        return dumb_fill<Sign>(generator, propagator, dir);
}

//+----------------------------------------------------------------------------+
//|      Leiserson, Prokop and Randall, 1998                                   |
//|      http://supertech.csail.mit.edu/papers/debruijn.pdf                    |
//+----------------------------------------------------------------------------+

template<typename T> FORCE_INLINE
size_t Bit::index_DeBruijn(T b)
{
        const size_t N = Log2SizeOf<T>::VALUE;

        b *= DeBruijn<N>::FORD_SEQUENCE;
        b >>= DeBruijn<N>::SHIFT;
        return DeBruijn<N>::PREFIX_TABLE[b];
}

//+----------------------------------------------------------------------------+
//|      Kernighan & Ritchie, The C programming language, 2nd Ed.              |
//|      https://chessprogramming.wikispaces.com/Population+Count              |
//+----------------------------------------------------------------------------+

template<typename T> FORCE_INLINE
size_t Bit::count_Kernighan(T b)
{
        size_t count = 0;
        for (; b; clear_lowest(b))
                ++count;
        return count;
}

//+-----------------------------------------------------------------------------+
//|       Chess Programming Wiki, "Dumb7Fill" algorithm                         |
//|       https://chessprogramming.wikispaces.com/Dumb7Fill                     |
//+-----------------------------------------------------------------------------+

template<bool Sign, typename T> FORCE_INLINE
T Bit::dumb_fill(T generator, T propagator, size_t dir)
{
        T flood = 0;
        while (generator) {
                flood |= generator;
                generator = Shift<Sign>()(generator, dir) & propagator;
        }
        return flood;
}
