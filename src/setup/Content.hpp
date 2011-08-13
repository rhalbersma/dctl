#include "../node/Material.h"

namespace dctl {
namespace setup {

template<typename Token>
std::string content(const Material& p, int b)
{
        const BitBoard bb = BitBoard(1) << b;

        std::stringstream sstr;
        if (p.pieces(Side::BLACK) & bb) {
                if (p.kings() & bb)
			sstr << Token::UPPER[Side::BLACK];      // black king
                else
                        sstr << Token::LOWER[Side::BLACK];      // black man
        } else if (p.pieces(Side::WHITE) & bb) {
                if (p.kings() & bb)
                        sstr << Token::UPPER[Side::WHITE];      // white king
                else
                        sstr << Token::LOWER[Side::WHITE];      // white man
        } else
                sstr << Token::EMPTY;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
