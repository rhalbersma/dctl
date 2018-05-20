//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board.hpp>
#include <dctl/core/rules.hpp>
#include <iostream>

using namespace dctl::core;

template<class CharT, class Traits, class Board>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, basic_board<Board> const& b)
{
        using Coord = coordinates<upper_left>;
        for (auto y = 0; y < b.height; ++y) {
                for (auto x = 0; x < b.width; ++x) {
                        if (auto const coord = Coord{x, y}; b.is_square(coord)) {
                                ostr << std::setw(2) << b.to_square(coord);
                        } else {
                                ostr << std::string(2, ' ');
                        }
                }
                ostr << '\n';
        }
        return ostr;
}

int main()
{
        std::cout << basic_board<checkers>{};
}
