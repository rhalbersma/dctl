#pragma once

//          Copyright Rein Halbersma 2010-2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/coordinates.hpp>      // coordinates, upper_left
#include <iomanip>                              // setw
#include <sstream>                              // stringstream
#include <string>                               // string

namespace dctl::core {

template<class Board>
struct diagram
{
        template<class UnaryFunction>
        auto operator()(UnaryFunction fun) const
        {
                std::stringstream sstr;
                for (auto r = Board::height - 1; r >= 0; --r) {
                        for (auto f = 0; f < Board::width; ++f) {
                                auto const coord = coordinates<lower_left>{f, r};
                                if (Board::is_square(coord)) {
                                        sstr << std::setw(2) << fun(Board::embedding0(Board::to_square(coord)));
                                } else {
                                        sstr << std::string(2, ' ');
                                }
                        }
                        sstr << '\n';
                }
                return sstr.str();
        }
};

}       // namespace dctl::core
