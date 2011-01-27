#include "BoardIO.h"
#include "../Board/Board.h"
#include "../Position/Position.h"
#include "../Position/Side.h"
#include "../Utilities/Bit.h"
#include "../Utilities/IntegerTypes.h"
#include <cctype>
#include <iomanip>
#include <sstream>

template<typename Board, typename Token>
Position<Board> read_position_string<DXP_tag, Board, Token>::operator()(const std::string& s)
{
	assert(s.length() == Board::SQUARE_RANGE + 1);

	BitBoard p_pieces[2] = {0, 0};
	BitBoard p_kings = 0;
	bool p_side = Side::BLACK;

	std::stringstream sstr(s);
	char ch;

	sstr >> ch;
	switch(ch) {
	case Token::BLACK:
		p_side = Side::BLACK;			// black to move
		break;
	case Token::WHITE:
		p_side = Side::WHITE;			// white to move
		break;
        default:
                assert(false);
	}

	BitBoard bb;
        size_t b;
	for (size_t i = 0; i < Board::SQUARE_RANGE; ++i) {
                b = Board::TABLE_SQUARE2BIT[i];         // convert square to bit
		bb = BitBoard(1) << b;                  // create bitboard
		sstr >> ch;
		switch(toupper(ch)) {
		case Token::BLACK:			
			p_pieces[Side::BLACK] ^= bb;    // black piece
			break;
		case Token::WHITE:			
			p_pieces[Side::WHITE] ^= bb;    // white piece
			break;
                case Token::EMPTY:
                        break;
                default:
                        assert(false);
		}
                if (isupper(ch))
                        p_kings ^= bb;                  // king
	}
	return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);
}

template<typename Token> template<typename Board>
std::string write_position_string<DXP_tag, Token>::operator()(const Position<Board>& p) const
{
	std::stringstream sstr;
	size_t b;

	sstr << Token::COLOR[p.to_move()];				// side to move
	for (size_t sq = 0; sq < Board::SQUARE_RANGE; ++sq) {
		b = Board::TABLE_SQUARE2BIT[sq];                        // convert square to bit
		sstr << write_position_bit<Board, Token>()(p, b);       // bit content
	}
	return sstr.str();
}