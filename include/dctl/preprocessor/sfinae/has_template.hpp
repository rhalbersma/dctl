#pragma once
#include <type_traits>                  // integral_constant

#define DCTL_PP_DEFINE_HAS_TEMPLATE(U)                                          \
template<class T, class... Args>                                                \
struct has_template_ ## U                                                       \
{                                                                               \
private:                                                                        \
        using yes = char;                                                       \
        using no = struct { char array[2]; };                                   \
                                                                                \
        template<class C> static yes test(typename C::template U<Args...>*);    \
        template<class C> static no  test(...);                                 \
public:                                                                         \
        enum { value = sizeof(test<T>(0)) == sizeof(yes) };                     \
        using type = std::integral_constant<bool, value>;                       \
};
