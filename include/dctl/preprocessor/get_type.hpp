#pragma once

#define DCTL_PP_DEFINE_GET_TYPE(U)      \
template<class T>                       \
struct get_ ## U                        \
{                                       \
        typedef typename T::U type;     \
};
