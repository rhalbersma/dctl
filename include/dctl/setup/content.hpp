#pragma once
#include <sstream>                      // stringstream
#include <dctl/position/color.hpp>       // black, white

namespace dctl {
namespace setup {

template<class Token, class Position>
std::string content(Position const& p, int i)
{
        std::stringstream sstr;
        if (p.pieces(Color::black).test(i)) {
                if (p.kings().test(i))
                        sstr << Token::upper[Color::black];     // black king
                else
                        sstr << Token::lower[Color::black];     // black pawn
        } else if (p.pieces(Color::white).test(i)) {
                if (p.kings().test(i))
                        sstr << Token::upper[Color::white];     // white king
                else
                        sstr << Token::lower[Color::white];     // white pawn
        } else
                sstr << Token::empty;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
