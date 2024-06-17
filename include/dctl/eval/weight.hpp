#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <array>        // array

namespace dctl::eval {

template<class Rules, class Board>
struct Weight
{
        static constexpr auto material          = std::array{ 100, 250 };
        static constexpr auto tempo             = std::array{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        static constexpr auto center            = std::array{ 0, 1, 2, 3, 4 };
        static constexpr auto balance           = std::array{ 5, 4, 3, 2, 1 };
        static constexpr auto mobility          = 1;
        static constexpr auto king_monopoly     = 200;
};

}       // namespace dctl::eval
