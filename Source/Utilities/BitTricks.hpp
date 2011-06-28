#include "DeBruijn.h"
#include "Shift.h"

namespace bit {

//+----------------------------------------------------------------------------+
//|      Leiserson, Prokop and Randall, 1998                                   |
//|      http://supertech.csail.mit.edu/papers/debruijn.pdf                    |
//+----------------------------------------------------------------------------+

template<typename T>
size_t index_DeBruijn(T b)
{
        b *= DeBruijn<T>::SEQUENCE;
        b >>= DeBruijn<T>::SHIFT;
        return DeBruijn<T>::TABLE[b];
}

//+----------------------------------------------------------------------------+
//|      Kernighan & Ritchie, The C programming language, 2nd Ed.              |
//|      https://chessprogramming.wikispaces.com/Population+Count              |
//+----------------------------------------------------------------------------+

template<typename T>
size_t count_Kernighan(T b)
{
        size_t count = 0;
        for (; b; bit::clear_lowest(b))
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
        T flood(0);
        while (generator) {
                flood |= generator;
                generator = Shift<Sign>()(generator, dir) & propagator;
        }
        return flood;
}

}       // namespace bit
