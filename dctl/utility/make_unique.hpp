#pragma once
#include <memory>                       // unique_ptr

namespace dctl {

#ifndef _MSC_VER

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return (std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
}

#else

template
<
        typename T
>
std::unique_ptr<T> make_unique()
{
        return (std::unique_ptr<T>(new T()));
}

template
<
        typename T,
        typename Arg1
>
std::unique_ptr<T> make_unique(Arg1&& a1)
{
        return (std::unique_ptr<T>(new T(std::forward<Arg1>(a1))));
}

template
<
        typename T,
        typename Arg1,
        typename Arg2
>
std::unique_ptr<T> make_unique(Arg1&& a1, Arg2&& a2)
{
        return (std::unique_ptr<T>(new T(std::forward<Arg1>(a1), std::forward<Arg2>(a2))));
}

#endif

}       // namespace dctl
