#pragma once
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
                        sstr << fun(Board::bit_from_square(sq));
                }
                return sstr.str();
        }
};

}       // namespace dctl::core
