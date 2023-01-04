#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>      // stringstream

namespace dctl::core {

template<class Board>
struct string
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction fun) const
        {
                std::stringstream sstr;
                for (auto sq = 0; sq < Board::size(); ++sq) {
                        sstr << fun(Board::embedding0(sq));
                }
                return sstr.str();
        }
};

}       // namespace dctl::core
