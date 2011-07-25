#include <functional>
#include <sstream>
#include "Position.h"
#include "Material.h"
#include "Side.h"
#include "../board/Diagram.h"
#include "../utils/Bit.h"
#include "../utils/IntegerTypes.h"

namespace dctl {

template<typename Protocol, typename Token> template<typename Board>
const std::string Diagram<Protocol, Token>::operator()(const Position<Board>& p) const
{
        return board::Diagram<Board, board::bits>()(std::bind(bit_content<Token>, p.material(), std::placeholders::_1));
}

template<typename Token>
std::string bit_content(const Material& p, size_t b)
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

}       // namespace dctl
