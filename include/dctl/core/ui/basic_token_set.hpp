#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <array>        // array
#include <cctype>       // tolower, toupper

namespace dctl::core {

template<char Black, char White, char Empty>
struct basic_token_set
{
        static constexpr auto black = Black;
        static constexpr auto white = White;
        static constexpr auto empty = Empty;

        static constexpr auto color = std::array
        {
                black,
                white
        };

        static inline const auto pawns = std::array
        {
                static_cast<char>(std::tolower(black)),
                static_cast<char>(std::tolower(white))
        };

        static inline const auto kings = std::array
        {
               static_cast<char>(std::toupper(black)),
               static_cast<char>(std::toupper(white))
        };
};

}       // namespace dctl::core
