#pragma once
#include <string>                       // std::string
#include "../node/Move.hpp"

namespace dctl {
namespace setup {

template<typename Token>
std::string content(const Move& p, int b)
{
        const BitBoard bb = bit::singlet<BitBoard>(b);

        std::stringstream sstr;
        if (p.pieces(Side::black) & bb) {
                if (p.kings() & bb)
			sstr << Token::upper[Side::black];      // black king
                else
                        sstr << Token::lower[Side::black];      // black man
        } else if (p.pieces(Side::white) & bb) {
                if (p.kings() & bb)
                        sstr << Token::upper[Side::white];      // white king
                else
                        sstr << Token::lower[Side::white];      // white man
        } else
                sstr << Token::empty;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
