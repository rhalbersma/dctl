#include "BoardIO.h"
#include "../Geometry/Board.h"
#include "../Tree/Node/Position.h"
#include "../Tree/Node/Side.h"
#include "../Utilities/Bit.h"
#include "../Utilities/IntegerTypes.h"
#include <cctype>
#include <iomanip>
#include <sstream>

template<typename Protocol, typename Token> template<typename Board>
std::string write_position_layout<Protocol, Token>::operator()(const Tree::Node::Position<Board>& p) const
{
        return write_bit_layout<Board>()(std::bind1st(write_position_bit<Board, Token>(), p));
}

template<typename Board, typename Token>
std::string write_position_bit<Board, Token>::operator()(const Tree::Node::Position<Board>& p, size_t b) const
{
        const BitBoard bb = BitBoard(1) << b;
        std::stringstream sstr;

        if (p.pieces(Tree::Node::Side::BLACK) & bb) {
                if (p.kings() & bb)
			sstr << Token::UPPER[Tree::Node::Side::BLACK];  // black king
                else
                        sstr << Token::LOWER[Tree::Node::Side::BLACK];  // black man
        } else if (p.pieces(Tree::Node::Side::WHITE) & bb) {
                if (p.kings() & bb)
                        sstr << Token::UPPER[Tree::Node::Side::WHITE];  // white king
                else
                        sstr << Token::LOWER[Tree::Node::Side::WHITE];  // white man
        } else
                sstr << Token::EMPTY;                                   // empty square

        return sstr.str();
}

template<typename Board>
bool is_valid_square(size_t sq)
{
        return sq < Board::SIZE;
}
