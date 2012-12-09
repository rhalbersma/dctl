#pragma once

#ifdef _MSC_VER

#define DCTL_PP_IS_DEFAULT {}
#define DCTL_PP_IS_DELETE ;

#else

#define DCTL_PP_IS_DEFAULT = default;
#define DCTL_PP_IS_DELETE = delete;

#endif
