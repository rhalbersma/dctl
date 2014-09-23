#pragma once
#include <type_traits>  // integral_constant

#define DCTL_PP_TTI_HAS_TYPE(NAME)                                              \
                                                                                \
namespace detail_ ## NAME {                                                     \
                                                                                \
template<class T>                                                               \
class has_type                                                                  \
{                                                                               \
private:                                                                        \
        using yes = char;                                                       \
        using no = yes (&)[2];                                                  \
                                                                                \
        template<class U>                                                       \
        static yes test(typename U::NAME*);                                     \
                                                                                \
        template<class U>                                                       \
        static no  test(...);                                                   \
                                                                                \
public:                                                                         \
        static constexpr auto value = sizeof(test<T>(nullptr)) == sizeof(yes);  \
        using type = std::integral_constant<bool, value>;                       \
};                                                                              \
                                                                                \
template<class T>                                                               \
using has_type_t = typename has_type<T>::type;                                  \
                                                                                \
template<class T>                                                               \
constexpr auto has_type_v = has_type_t<T>::value;                               \
                                                                                \
}       /* namespace detail_ ## NAME */
