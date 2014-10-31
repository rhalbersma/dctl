#pragma once

#define DCTL_PP_TTI_BOX_TYPE(NAME)      \
                                        \
template<class T>                       \
struct box_type_ ## NAME                \
{                                       \
        using type = typename T::NAME;  \
};                                      \
