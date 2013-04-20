#pragma once

#define DCTL_PP_DEFINE_GET_TYPE(U)      \
template<typename T>                    \
struct get_ ## U                        \
{                                       \
        typedef typename T::U type;     \
};
