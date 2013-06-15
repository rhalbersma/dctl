#pragma once
#include <type_traits>                  // integral_constant

#define DCTL_PP_DEFINE_HAS_TEMPLATE(U)                                          \
template<class T, class... Args>                                                \
struct has_template_ ## U                                                       \
{                                                                               \
private:                                                                        \
        typedef char                      yes;                                  \
        typedef struct { char array[2]; } no;                                   \
                                                                                \
        template<class C> static yes test(typename C::template U<Args...>*);    \
        template<class C> static no  test(...);                                 \
public:                                                                         \
        enum { value = sizeof(test<T>(0)) == sizeof(yes) };                     \
        typedef std::integral_constant<bool, value> type;                       \
};
