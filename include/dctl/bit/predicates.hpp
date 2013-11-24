#pragma once
#include <dctl/bit/algorithm.hpp>

namespace dctl {
namespace bit {

template<class T>
bool is_single(T b)
{
        return bit::set_is_count_equal_to(b, 1);
}

template<class T>
bool is_double(T b)
{
        return bit::set_is_count_equal_to(b, 2);
}

template<class T>
bool is_multiple(T b)
{
        return !bit::set_is_count_less(b, 2);
}

}       // namespace bit
}       // namespace dctl
