#pragma once
#include <type_traits>                  // integral_constant

#define DCTL_PP_DEFINE_HAS_TYPE(U)                              \
template<class T>                                               \
struct has_ ## U                                                \
{                                                               \
private:                                                        \
        typedef char                      yes;                  \
        typedef struct { char array[2]; } no;                   \
                                                                \
        template<class C> static yes test(typename C::U*);      \
        template<class C> static no  test(...);                 \
                                                                \
public:                                                         \
        enum { value = sizeof(test<T>(0)) == sizeof(yes) };     \
        typedef std::integral_constant<bool, value> type;       \
};
