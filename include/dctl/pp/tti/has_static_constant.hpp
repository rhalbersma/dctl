#pragma once
#include <type_traits>                  // integral_constant

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
