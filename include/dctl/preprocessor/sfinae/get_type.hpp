#pragma once

#define DCTL_PP_DEFINE_GET_TYPE(U)      \
template<class T>                       \
struct get_type_ ## U                   \
{                                       \
        using type = typename T::U;     \
};
