#pragma once
#include <type_traits>	// conditional, enable_if_t, integral_constant, is_integral, remove_const_t

template<class T>
static constexpr auto is_integral_v = std::is_integral<T>::value;

#define DCTL_PP_TTI_HAS_NO_DEFAULT nullptr

#define DCTL_PP_TTI_HAS_STATIC_CONSTANT(NAME)                           \
                                                                        \
namespace detail_ ## NAME  {                                            \
                                                                        \
template<class T>                                                       \
class has_static_constant                                               \
{                                                                       \
private:                                                                \
        using yes = char;                                               \
        using no = yes (&)[2];                                          \
                                                                        \
        template<class U>                                               \
        static yes test(decltype(U::NAME)*);                            \
                                                                        \
        template<class U>                                               \
        static no  test(...);                                           \
                                                                        \
public:                                                                 \
        enum { value = sizeof(test<T>(0)) == sizeof(yes) };             \
        using type = std::integral_constant<bool, value>;               \
};                                                                      \
                                                                        \
template<class T>                                                       \
using has_static_constant_t = typename has_static_constant<T>::type;    \
                                                                        \
template<class T>                                                       \
constexpr auto has_static_constant_v = has_static_constant_t<T>::value; \
                                                                        \
}       /* namespace detail_ ## NAME */

#define DCTL_PP_TTI_STATIC_CONSTANT(NAME, VALUE)                        \
                                                                        \
DCTL_PP_TTI_HAS_STATIC_CONSTANT(NAME)                                   \
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
