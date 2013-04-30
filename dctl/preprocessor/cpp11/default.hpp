#pragma once

#ifdef _MSC_VER
        #define DCTL_PP_IS_DEFAULT {}
#else
        #define DCTL_PP_IS_DEFAULT = default;
#endif
