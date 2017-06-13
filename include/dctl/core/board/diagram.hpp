#pragma once
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
                for (auto y = 0; y < Board::height; ++y) {
                        for (auto x = 0; x < Board::width; ++x) {
                                auto const coord = coordinates<upper_left>{x, y};
                                if (Board::is_square(coord)) {
                                        sstr << std::setw(2) << fun(Board::bit_from_square(Board::to_square(coord)));
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
