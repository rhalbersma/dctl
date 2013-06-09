#pragma once
#include <dctl/utility/int.hpp>

namespace dctl {
namespace bit {

template<typename T>
T zero()
{
        return {0};
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

}       // namespace bit
}       // namespace dctl
