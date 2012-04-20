#pragma once
#include <sstream>                      // stringstream
#include "../node/Material.hpp"

namespace dctl {
namespace setup {

template<typename Token>
std::string content(const Material& m, int i)
{
        const auto b = bit::singlet<BitBoard>(i);

        std::stringstream sstr;
        if (m.pieces(Side::black) & b) {
                if (m.kings() & b)
                        sstr << Token::upper[Side::black];      // black king
                else
                        sstr << Token::lower[Side::black];      // black man
        } else if (m.pieces(Side::white) & b) {
                if (m.kings() & b)
                        sstr << Token::upper[Side::white];      // white king
                else
                        sstr << Token::lower[Side::white];      // white man
        } else
                sstr << Token::empty;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
