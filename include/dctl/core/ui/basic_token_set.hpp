#pragma once

//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cctype>       // tolower, toupper

namespace dctl::core {

template<char Black, char White, char Empty>
struct basic_token_set
{
        constexpr static auto black = Black;
        constexpr static auto white = White;
        constexpr static auto empty = Empty;

        inline const static char color[] =
        {
                black,
                white
        };

        inline const static char pawns[] =
        {
                static_cast<char>(std::tolower(black)),
                static_cast<char>(std::tolower(white))
        };

        inline const static char kings[] =
        {
               static_cast<char>(std::toupper(black)),
               static_cast<char>(std::toupper(white))
        };
};

}       // namespace dctl::core
