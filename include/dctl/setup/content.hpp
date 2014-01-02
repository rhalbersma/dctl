#pragma once
#include <sstream>                      // stringstream
#include <dctl/position/side.hpp>       // black, white

namespace dctl {
namespace setup {

template<class Token, class Position>
std::string content(Position const& p, int i)
{
        std::stringstream sstr;
        if (p.pieces(Side::black).test(i)) {
                if (p.kings().test(i))
                        sstr << Token::upper[Side::black];      // black king
                else
                        sstr << Token::lower[Side::black];      // black man
        } else if (p.pieces(Side::white).test(i)) {
                if (p.kings().test(i))
                        sstr << Token::upper[Side::white];      // white king
                else
                        sstr << Token::lower[Side::white];      // white man
        } else
                sstr << Token::empty;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
