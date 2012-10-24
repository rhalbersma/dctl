#pragma once
#include <memory>                       // unique_ptr

namespace dctl {

// TODO: generalize this to multiple arguments using variadic templates
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

}       // namespace dctl
