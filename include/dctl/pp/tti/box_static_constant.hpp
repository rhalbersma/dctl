#pragma once
#include <type_traits>                  // integral_constant, remove_const

#define DCTL_PP_TTI_BOX_STATIC_CONSTANT(NAME)                           \
                                                                        \
namespace detail_ ## NAME  {                                            \
                                                                        \
template<class T>                                                       \
struct box_static_constant                                              \
{                                                                       \
        static constexpr auto value = T::NAME;                          \
        using value_type = std::remove_const_t<decltype(value)>;        \
        using type = std::integral_constant<value_type, value>;         \
};                                                                      \
                                                                        \
}       /* namespace detail_ ## NAME */
