#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define DCTL_PP_TTI_CONSTANT(NAME, DEFAULT)     \
                                                \
template<class T>                               \
inline constexpr auto NAME ## _v = []() {       \
        if constexpr (requires { T::NAME; }) {  \
            return T::NAME;                     \
        } else {                                \
            return DEFAULT;                     \
        }                                       \
}();                                            \

