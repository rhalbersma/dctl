#include "BoardIO.h"
#include "../Geometry/Board.h"
#include "../Tree/Node/Position.h"
#include "../Tree/Node/Side.h"
#include "../Utilities/Bit.h"
#include "../Utilities/IntegerTypes.h"
#include <cctype>
#include <iomanip>
#include <sstream>

template<typename Board, typename Token>
Position<Board> read_position_string<Board, DXP_tag, Token>::operator()(const std::string& s)
{
	assert(s.length() == Board::SIZE + 1);

	BitBoard p_pieces[2] = {0, 0};
	BitBoard p_kings = 0;
	bool p_side = Node::Side::BLACK;

	std::stringstream sstr(s);
	char ch;

	sstr >> ch;
	switch(ch) {
	case Token::BLACK:
		p_side = Node::Side::BLACK;		        // black to move
		break;
	case Token::WHITE:
		p_side = Node::Side::WHITE;                     // white to move
		break;
        default:
                assert(false);
	}

	BitBoard bb;
        size_t b;
	for (size_t i = 0; i < Board::SIZE; ++i) {
                b = Board::TABLE_SQUARE2BIT[i];                 // convert square to bit
		bb = BitBoard(1) << b;                          // create bitboard
		sstr >> ch;
		switch(toupper(ch)) {
		case Token::BLACK:			
			p_pieces[Node::Side::BLACK] ^= bb;      // black piece
			break;
		case Token::WHITE:			
			p_pieces[Node::Side::WHITE] ^= bb;      // white piece
			break;
                case Token::EMPTY:
                        break;
                default:
                        assert(false);
		}
                if (isupper(ch))
                        p_kings ^= bb;                          // king
	}
	return Position<Board>(p_pieces[Node::Side::BLACK], p_pieces[Node::Side::WHITE], p_kings, p_side);
}

template<typename Token> template<typename Board>
std::string write_position_string<DXP_tag, Token>::operator()(const Node::Position<Board>& p) const
{
	std::stringstream sstr;
	size_t b;

	sstr << Token::COLOR[p.to_move()];				// side to move
	for (size_t sq = 0; sq < Board::SIZE; ++sq) {
		b = Board::TABLE_SQUARE2BIT[sq];                        // convert square to bit
		sstr << write_position_bit<Board, Token>()(p, b);       // bit content
	}
	return sstr.str();
}
