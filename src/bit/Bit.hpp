#include <cassert>
#include "DeBruijn.h"
#include "Lookup.h"

namespace dctl {
namespace bit {

template<typename T>
bool is_zero(T b)
{
        return b == T(0);
}

template<typename T>
bool is_single(T b)
{
        return !(is_zero(b) || is_multiple(b));
}

template<typename T>
bool is_double(T b)
{
        return is_single(except_first(b));
}

template<typename T>
bool is_multiple(T b)
{
        return !is_zero(except_first(b));
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
T get_first(T b)
{
        return b & (T(0) - b);
}

template<typename T>
T except_first(T b)
{
        return b & (b - T(1));
}

template<typename T>
int find_first(T b)
{
        return index(get_first(b));
}

template<typename T>
int index(T b)
{
	assert(is_single(b));
        return index_DeBruijn(b);
}

//+----------------------------------------------------------------------------+
//|      Leiserson, Prokop and Randall, 1998                                   |
//|      http://supertech.csail.mit.edu/papers/debruijn.pdf                    |
//+----------------------------------------------------------------------------+

template<typename T>
int index_DeBruijn(T b)
{
        return DeBruijn<T>::index(b);
}

template<typename T>
int index_lookup(T b)
{
        return Lookup<uint8_t>::index(b);
}

template<typename T>
int count(T b)
{
        return count_lookup(b);        
}

//+----------------------------------------------------------------------------+
//|      Kernighan & Ritchie, The C programming language, 2nd Ed.              |
//|      https://chessprogramming.wikispaces.com/Population+Count              |
//+----------------------------------------------------------------------------+

template<typename T>
int count_Kernighan(T b)
{
        int count = 0;
        for (; b; clear_first(b))
                ++count;
        return count;
}

template<typename T>
int count_lookup(T b)
{
        return Lookup<uint8_t>::count(b);
}

template<bool Sign, typename T>
T flood_fill(T generator, T propagator, int dir)
{
        return fill_loop<Sign>(generator, propagator, dir);
}

//+-------------------------------------------------------------------------------------+
//|       Chess Programming Wiki, "Fill Loop" algorithm                                 |
//|       http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop  |
//+-------------------------------------------------------------------------------------+

template<bool Sign, typename T>
T fill_loop(T generator, T propagator, int dir)
{
        T flood(0);
        while (generator) {
                flood |= generator;
                generator = Shift<Sign>()(generator, dir) & propagator;
        }
        return flood;
}

template<typename T>
void clear_first(T& b)
{
        b &= b - T(1);
}

}       // namespace bit
}       // namespace dctl
