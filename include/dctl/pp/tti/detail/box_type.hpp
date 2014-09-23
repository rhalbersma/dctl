#pragma once

#define DCTL_PP_TTI_BOX_TYPE(NAME)      \
                                        \
namespace detail_ ## NAME {             \
                                        \
template<class T>                       \
struct box_type                         \
{                                       \
        using type = typename T::NAME;  \
};                                      \
                                        \
}       /* namespace detail_ ## NAME */
