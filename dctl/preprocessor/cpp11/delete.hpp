#pragma once

#ifdef _MSC_VER
        #define DCTL_PP_IS_DELETE ;
#else
        #define DCTL_PP_IS_DELETE = delete;
#endif
