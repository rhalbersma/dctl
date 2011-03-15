#include "DeBruijn.h"
#include "Shift.h"

namespace Bit {

//+----------------------------------------------------------------------------+
//|      Leiserson, Prokop and Randall, 1998                                   |
//|      http://supertech.csail.mit.edu/papers/debruijn.pdf                    |
//+----------------------------------------------------------------------------+

template<typename T>
size_t index_DeBruijn(T b)
{
        const size_t N = log2_sizeof<T>::value;         // compute number of bits of type T
        b *= static_cast<T>(DeBruijn<N>::SEQUENCE);     // cast from the internal 64-bit int to type T
        b >>= DeBruijn<N>::SHIFT;
        return DeBruijn<N>::TABLE[b];
}

//+----------------------------------------------------------------------------+
//|      Kernighan & Ritchie, The C programming language, 2nd Ed.              |
//|      https://chessprogramming.wikispaces.com/Population+Count              |
//+----------------------------------------------------------------------------+

template<typename T>
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

template<bool Sign, typename T>
T fill_loop(T generator, T propagator, size_t dir)
{
        T flood = 0;
        while (generator) {
                flood |= generator;
                generator = Shift<Sign>()(generator, dir) & propagator;
        }
        return flood;
}

}       // namespace Bit
