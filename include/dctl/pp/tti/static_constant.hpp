#pragma once
#include <dctl/pp/tti/box_static_constant.hpp>
#include <dctl/pp/tti/has_static_constant.hpp>
#include <type_traits>	                        // conditional, enable_if_t, integral_constant, is_integral

template<class T>
static constexpr auto is_integral_v = std::is_integral<T>::value;

#define DCTL_PP_TTI_HAS_NO_DEFAULT nullptr

#define DCTL_PP_TTI_STATIC_CONSTANT(NAME, VALUE)                        \
                                                                        \
DCTL_PP_TTI_HAS_STATIC_CONSTANT(NAME)                                   \
DCTL_PP_TTI_BOX_STATIC_CONSTANT(NAME)                                   \
                                                                        \
namespace detail_ ## NAME  {                                            \
                                                                        \
template<class T, class = void>                                         \
struct default_constant                                                 \
{                                                                       \
        static_assert(                                                  \
                is_integral_v<T>,                                       \
                "DCLT error: '" #NAME "' is not a member."              \
        );                                                              \
};                                                                      \
                                                                        \
template<class T>                                                       \
struct default_constant<T, std::enable_if_t<is_integral_v<T>>>          \
:                                                                       \
        std::integral_constant<T, VALUE>                                \
{};                                                                     \
                                                                        \
}       /* namespace detail_ ## NAME */                                 \
                                                                        \
template<class T>                                                       \
struct NAME                                                             \
:                                                                       \
        std::conditional<                                               \
                detail_ ## NAME ::has_static_constant_v<T>,             \
                detail_ ## NAME ::box_static_constant<T>,               \
                detail_ ## NAME ::default_constant<decltype(VALUE)>     \
        >::type                                                         \
{};                                                                     \
                                                                        \
template<class T>                                                       \
using NAME ## _t = typename NAME<T>::type;                              \
                                                                        \
template<class T>                                                       \
constexpr auto NAME ## _v = NAME ## _t<T>::value;
