#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if defined(__GNUG__)

#define DCTL_PP_CONSTEXPR_INTRINSIC constexpr

#elif defined(_MSC_VER)

#define DCTL_PP_CONSTEXPR_INTRINSIC inline const

#endif
