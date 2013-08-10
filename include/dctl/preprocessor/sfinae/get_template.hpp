#pragma once

#define DCTL_PP_DEFINE_GET_TEMPLATE(U)                  \
template<class T, class... Args>                        \
struct get_template_ ## U                               \
{                                                       \
        using type = typename T::template U<Args...>;   \
};
