#pragma once
#include <dctl/utility/int.hpp>

namespace dctl {
namespace bit {

template<class T>
T zero()
{
        return {0};
}

template<class T>
T universe()
{
        return ~zero<T>();
}

template<class T>
T singlet(int i)
{
        return T(1) << i;
}

template<class T>
T reverse_singlet(int i)
{
        return singlet<T>(num_bits<T>::value - 1 - i);
}

}       // namespace bit
}       // namespace dctl
