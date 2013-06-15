#pragma once
#include <dctl/bit/raw.hpp>
#include <dctl/bit/elementary.hpp>
#include <dctl/bit/first.hpp>

namespace dctl {
namespace bit {

// 2 or more bits set to 1
template<class T>
bool is_multiple(T b)
{
        return !empty(first::not_equal_to(b));
}

template<class Iterator, class Board>
bool is_element(Iterator it, Board b)
{
        return !empty(it & b);
}

// 1 bit set to 1
template<class T>
bool is_single(T b)
{
        return !(empty(b) || is_multiple(b));
}

// 2 bits set to 1
template<class T>
bool is_double(T b)
{
        return is_single(first::not_equal_to(b));
}

}       // namespace bit
}       // namespace dctl
