#pragma once

//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sstream>                      // stringstream
#include <stdexcept>                    // invalid_argument
#include <string>                       // string

namespace dctl::algo {

struct Bound
{
        enum : unsigned {
                lower = 1,
                upper = 2,
                exact = lower ^ upper
        };

        static auto type(int value, int alpha, int beta)
        {
                return
                        value >= beta  ? lower :
                        value <= alpha ? upper :
                                         exact
                ;
        }

        static std::string print(unsigned bound)
        {
                std::stringstream sstr;
                switch (bound) {
                case lower: sstr << ">="; break;
                case upper: sstr << "<="; break;
                case exact: sstr << "=="; break;
                default: throw std::invalid_argument("Search bound must be >=, <= or ==");
                }
                return sstr.str();
        }
};

}       // namespace dctl::algo
