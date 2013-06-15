#pragma once
#include <dctl/bit/elementary.hpp>

namespace dctl {
namespace bit {
namespace range {

// [i, i+1)
template<class T>
T equal(int i)
{
        return singlet<T>(i);
}

// [0, ..., i-1, i+1, ... N)
template<class T>
T not_equal_to(int i)
{
        return ~equal<T>(i);
}

// [0, i)
template<class T>
T less(int i)
{
        return equal<T>(i) - 1;
}

// [i, N)
template<class T>
T greater_equal(int i)
{
        return ~less<T>(i);
}

// [0, i+1)
template<class T>
T less_equal(int i)
{
        return less<T>(i + 1);
}

// [i+1, N)
template<class T>
T greater(int i)
{
        return ~less_equal<T>(i);
}

}       // namespace range
}       // namespace bit
}       // namespace dctl
