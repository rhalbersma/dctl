#pragma once

#define DCTL_PP_TTI_BOX_TEMPLATE(U)                     \
template<class T, class... Args>                        \
struct box_template_ ## U                               \
{                                                       \
        using type = typename T::template U<Args...>;   \
};
