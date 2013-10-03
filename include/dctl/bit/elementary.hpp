#pragma once
#include <limits>                       // digits

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
        return T{1} << i;
}

template<class T>
T reverse_singlet(int i)
{
        return singlet<T>(std::numeric_limits<T>::digits - 1 - i);
}

}       // namespace bit
}       // namespace dctl
