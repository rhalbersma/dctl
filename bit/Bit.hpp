#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "DeBruijn.hpp"
#include "Lookup.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace bit {

template<typename T>
T singlet(int i)
{
        return T(1) << i;
}

template<typename T>
T reverse_singlet(int i)
{
        return singlet<T>(num_bits<T>::value - 1 - i);
}

// 0 bits set to 1
template<typename T>
bool is_zero(T b)
{
        return b == T(0);
}

// 1 bit set to 1
template<typename T>
bool is_single(T b)
{
        return !(is_zero(b) || is_multiple(b));
}

// 2 bits set to 1
template<typename T>
bool is_double(T b)
{
        return is_single(except_first(b));
}

// 2 or more bits set to 1
template<typename T>
bool is_multiple(T b)
{
        return !is_zero(except_first(b));
}

// a contained within b
template<typename T>
bool is_subset_of(T a, T b)
{
        return is_zero(a & ~b);
}

// a and b mutually exclusive
template<typename T>
bool is_exclusive(T a, T b)
{
        return is_zero(a & b);
}

// least significant 1-bit
template<typename T>
T get_first(T b)
{
        return b & (T(0) - b);
}

// most significant 1-bits
template<typename T>
T except_first(T b)
{
        return b & (b - T(1));
}

// clear the least significant 1-bit
template<typename T>
void clear_first(T& b)
{
        b &= b - T(1);
}

// index of the least significant 1-bit
template<typename T>
int find_first(T b)
{
        return index(get_first(b));
}

// index of a set 1-bit
template<typename T>
int index(T b)
{
        BOOST_ASSERT(is_single(b));
        return index_DeBruijn(b);
}

// Leiserson, Prokop and Randall, 1998
// http://supertech.csail.mit.edu/papers/debruijn.pdf
template<typename T>
int index_DeBruijn(T b)
{
        return debruijn::Index<T>()(b);
}

// index of a set 1-bit
template<typename T>
int index_lookup(T b)
{
        return Lookup::index(b);
}

// index of a set 1-bit
template<typename T>
int index_loop(T b)
{
        for (auto i = 0; i < num_bits<T>::value; ++i)
                if (b & singlet<T>(i))
                        return i;
        return 0;
}

// number of set 1-bits
template<typename T>
int count(T b)
{
        return count_lookup(b);
}

// number of set 1-bits
template<typename T>
int count_lookup(T b)
{
        return Lookup::count(b);
}

// Kernighan & Ritchie, The C programming language, 2nd Ed.
// https://chessprogramming.wikispaces.com/Population+Count
template<typename T>
int count_loop(T b)
{
        auto count = 0;
        for (; b; clear_first(b))
                ++count;
        return count;
}

// direction-wise flood-fill generator over propagator
template<bool Sign, typename T>
T flood_fill(T generator, T propagator, int dir)
{
        return fill_loop<Sign>(generator, propagator, dir);
}

// Chess Programming Wiki, "Fill Loop" algorithm
// http://chessprogramming.wikispaces.com/Dumb7Fill#Occluded%20Fill-Fill%20Loop
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

}       // namespace bit
}       // namespace dctl
