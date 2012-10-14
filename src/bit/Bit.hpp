#pragma once
#include <functional>                   // function
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "Bit_fwd.hpp"                  // forward declarations
#include "Loop.hpp"                     // count, index
#include "../utility/IntegerTypes.hpp"  // num_bits

namespace dctl {
namespace bit {

template<typename T>
T zero()
{
        return T(0);
}

template<typename T>
T universe()
{
        return ~zero<T>();
}

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
      
namespace first {
namespace detail {

template<typename T>
struct equal
:
        std::function<T(T)>
{
        T operator()(T b) const
        {
                return b & (T(0) - b);
        }
};

template<typename T>
struct not_equal_to
:
        std::function<T(T)>
{
        T operator()(T b) const
        {
                return b & (b - T(1));
        }
};

template<typename T>
struct less
:
        std::function<T(T)>
{
        T operator()(T b) const
        {
                return ~b & (b - T(1));
        }
};

template<typename T>
struct less_equal
:
        std::function<T(T)>
{
        T operator()(T b) const
        {
                return b ^ (b - T(1));
        }
};

template<typename T>
struct clear
:
        std::function<void(T&)>
{
        void operator()(T& b) const
        {
                b &= b - T(1);
        }
};

// TODO: partial specializations for bit arrays

}       // namespace detail

// least significant 1-bit
template<typename T>
T equal(T b)
{
        return detail::equal<T>()(b);
}

// most significant 1-bits
template<typename T>
T not_equal_to(T b)
{
        return detail::not_equal_to<T>()(b);
}

// index of the least significant 1-bit
template<typename T>
int find(T b)
{
        return index(equal(b));
}

// clear the least significant 1-bit
template<typename T>
void clear(T& b)
{
        detail::clear<T>()(b);
}

}       // namespace first

namespace range {

// [i, i+1)
template<typename T>
T equal(int i)
{
        return singlet<T>(i);
}

// [0, ..., i-1, i+1, ... N)
template<typename T>
T not_equal_to(int i)
{
        return ~equal<T>(i);
}

// [i+1, N)
template<typename T>
T greater(int i)
{
        return ~less_equal<T>(i);
}

// [0, i)
template<typename T>
T less(int i)
{
        return equal<T>(i) - 1;
}

// [i, N)
template<typename T>
T greater_equal(int i)
{
        return ~less<T>(i);
}

// [0, i] == [0, i+1)
template<typename T>
T less_equal(int i)
{
        return less<T>(i + 1);
}

}       // namespace range         
        
// number of set 1-bits
template<typename T>
int count(T b)
{
        return loop::count(b);
}
 
// index of a set 1-bit
template<typename T>
int index(T b)
{
        BOOST_ASSERT(is_single(b));
        return loop::index(b);
}

// 0 bits set to 1
template<typename T>
bool is_zero(T b)
{
        return b == zero<T>();
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
        return is_single(first::not_equal_to(b));
}

// 2 or more bits set to 1
template<typename T>
bool is_multiple(T b)
{
        return !is_zero(first::not_equal_to(b));
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

template<typename Iterator, typename Board>
bool is_element(Iterator it, Board b)
{
        return !is_zero(it & b);
}

}       // namespace bit
}       // namespace dctl
