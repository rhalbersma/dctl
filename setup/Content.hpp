#pragma once
#include <string>                       // std::string
#include "../node/Material.h"

namespace dctl {
namespace setup {

template<typename Token>
std::string content(const Material& p, int b)
{
        const BitBoard bb = BitBoard(1) << b;

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
