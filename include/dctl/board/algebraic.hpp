#pragma once
#include <boost/preprocessor/repetition/enum.hpp>

namespace dctl {
namespace board {

template<class Board>
struct Labels
{
        static int const row[];
        static char const col[];
};

#ifndef DCTL_PP_ROW_LABEL
    #define DCTL_PP_ROW_LABEL(z, i, data) 1 + i
#endif

template<class Board>
int const Labels<Board>::row[] =
{
        BOOST_PP_ENUM(26, DCTL_PP_ROW_LABEL, ~)
};

#undef DCTL_PP_ROW_LABEL

#ifndef DCTL_PP_COLUMN_LABEL
    #define DCTL_PP_COLUMN_LABEL(z, i, data) static_cast<char>('a' + i)
#endif

template<class Board>
char const Labels<Board>::col[] =
{
        BOOST_PP_ENUM(26, DCTL_PP_COLUMN_LABEL, ~)
};

#undef DCTL_PP_COLUMN_LABEL

}       // namespace board
}       // namespace dctl
