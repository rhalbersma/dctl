#pragma once
#include <string>                       // string
#include "../node/Move.hpp"

namespace dctl {
namespace setup {

template<typename Token>
std::string content(const Move& p, int i)
{
        const auto b = bit::singlet<BitBoard>(i);

        std::stringstream sstr;
        if (p.pieces(Side::black) & b) {
                if (p.kings() & b)
			sstr << Token::upper[Side::black];      // black king
                else
                        sstr << Token::lower[Side::black];      // black man
        } else if (p.pieces(Side::white) & b) {
                if (p.kings() & b)
                        sstr << Token::upper[Side::white];      // white king
                else
                        sstr << Token::lower[Side::white];      // white man
        } else
                sstr << Token::empty;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
