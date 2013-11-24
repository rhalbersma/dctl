#pragma once
#include <dctl/bit/elementary.hpp>
#include <dctl/bit/first.hpp>

namespace dctl {
namespace bit {

template<class T>
bool empty(T b)
{
        return b == T{0};
}

// set[i] == true
template<class Iterator, class Board>
bool is_element(Iterator it, Board b)
{
        return !empty(it & b);
}

// !set_count_less(2)
template<class T>
bool is_multiple(T b)
{
        return !empty(first::not_equal_to(b));
}

// set_count_equal_to(1)
template<class T>
bool is_single(T b)
{
        return !(empty(b) || is_multiple(b));
}

// set_count_equal_to(2)
template<class T>
bool is_double(T b)
{
        return is_single(first::not_equal_to(b));
}

}       // namespace bit
}       // namespace dctl
