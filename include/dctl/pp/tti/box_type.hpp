#pragma once

#define DCTL_PP_TTI_BOX_TYPE(U)         \
template<class T>                       \
struct box_type_ ## U                   \
{                                       \
        using type = typename T::U;     \
};
