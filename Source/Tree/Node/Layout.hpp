#include <functional>
#include <sstream>
#include "Side.h"
#include "../../Geometry/Layout.h"
#include "../../Utilities/Bit.h"
#include "../../Utilities/IntegerTypes.h"

namespace Tree {
namespace Node {
namespace Layout {

template<typename Protocol, typename Token> template<typename Board>
std::string write<Protocol, Token>::operator()(const Position<Board>& p) const
{
        return Geometry::Layout::write<Board, Geometry::Layout::Bit_tag>()(std::bind(bit_content<Token>, p.pieces(), std::placeholders::_1));
}

template<typename Token>
std::string bit_content(const Pieces& p, size_t b)
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

}       // namespace Layout
}       // namespace Node
}       // namespace Tree
