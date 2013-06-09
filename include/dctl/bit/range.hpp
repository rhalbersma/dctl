#pragma once
#include <dctl/bit/elementary.hpp>

namespace dctl {
namespace bit {
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

// [i+1, N)
template<typename T>
T greater(int i)
{
        return ~less_equal<T>(i);
}

}       // namespace range
}       // namespace bit
}       // namespace dctl
