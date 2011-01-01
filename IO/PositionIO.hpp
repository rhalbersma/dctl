#include "BoardIO.h"
#include "../Board/Board.h"
#include "../Position/Position.h"
#include "../Position/Reversible/Side.h"
#include "../Utilities/Bit.h"
#include "../Utilities/IntegerTypes.h"
#include <cctype>
#include <iomanip>
#include <sstream>

//+----------------------------------------------------------------------------+

template<typename Board, typename Token>
Position<Board> read_position_string<FEN, Board, Token>::operator()(const std::string& s)
{      
        BitBoard p_pieces[2] = {0, 0};
	BitBoard p_kings = 0;
	bool p_side = Side::BLACK;

        // do not attempt to parse empty strings
        if (s.empty())
                return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);

        bool setup_kings = false;
        bool setup_color = p_side;

        std::stringstream sstr(s);
        char ch;
        size_t sq;
        size_t b;
        BitBoard bb;

	sstr >> ch;
	switch(ch) {
	case Token::BLACK:
		p_side = Side::BLACK;			                // black to move
		break;
	case Token::WHITE:
		p_side = Side::WHITE;			                // white to move
		break;
        default:
                assert(false);
	}

        for (sstr >> ch; sstr; sstr >> ch) {
                switch(ch) {
                case Token::COLON:
                        sstr >> ch;
                        switch(ch) {
                        case Token::BLACK:                              // setup black pieces
                                setup_color = Side::BLACK;
                                break;
                        case Token::WHITE:                              // setup white pieces
                                setup_color = Side::WHITE;
                                break;
                        default:
                                assert(false);
                        }
                        break;
                case Token::KING:                                       // setup kings
                        setup_kings = true;
                        break;
                case Token::COMMA:
                        break;
                default:
                        if (isdigit(ch)) {
                                sstr.putback(ch);
                                sstr >> sq;                             // read square
			        assert(is_valid_square<Board>(sq - 1)); 
                                b = Board::TABLE_SQUARE2BIT[sq - 1];    // convert square to bit
			        bb = BitBoard(1) << b;                  // create bitboard
                                p_pieces[setup_color] ^= bb;
                                if (setup_kings)
                                        p_kings ^= bb;
                        }
                        setup_kings = false;
                }
        }
        return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);
}

template<typename Token> template<typename Board>
std::string write_position_string<FEN, Token>::operator()(const Position<Board>& p) const
{
        std::stringstream sstr;
        size_t b;
	bool c;

	sstr << Token::QUOTE;					        // opening quotes
	sstr << Token::COLOR[p.to_move()];				// side to move
	for (size_t i = 0; i < 2; ++i) {
		c = i != 0;
		if (p.pieces(c)) {
			sstr << Token::COLON;                           // colon
			sstr << Token::COLOR[c];                        // color tag
		}
		for (BitBoard bb = p.pieces(c); bb; Bit::clear_lowest(bb)) {
			if (p.kings() & Bit::get_lowest(bb))
				sstr << Token::KING;			// king tag
                        b = Bit::scan_forward(bb);                      // bit index                        
			sstr << Board::TABLE_BIT2SQUARE[b] + 1;	        // square number
			if (Bit::is_multiple(bb))                       // still pieces remaining
				sstr << Token::COMMA;			// comma separator
		}
	}
	sstr << Token::QUOTE;						// closing quotes
	return sstr.str();
}

//+----------------------------------------------------------------------------+

template<typename Board, typename Token>
Position<Board> read_position_string<DXP, Board, Token>::operator()(const std::string& s)
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
                b = Board::TABLE_SQUARE2BIT[i];         // covnert square to bit
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
std::string write_position_string<DXP, Token>::operator()(const Position<Board>& p) const
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

//+----------------------------------------------------------------------------+

template<typename Protocol, typename Token> template<typename Board>
std::string write_position_layout<Protocol, Token>::operator()(const Position<Board>& p) const
{
        return write_bit_layout<Board>()(std::bind1st(write_position_bit<Board, Token>(), p));
}

//+----------------------------------------------------------------------------+

template<typename Board, typename Token>
std::string write_position_bit<Board, Token>::operator()(const Position<Board>& p, size_t b) const
{
        const BitBoard bb = BitBoard(1) << b;
        std::stringstream sstr;

        if (p.pieces(Side::BLACK) & bb) {
                if (p.kings() & bb)
			sstr << Token::UPPER[Side::BLACK];              // black king
                else
                        sstr << Token::LOWER[Side::BLACK];              // black man
        } else if (p.pieces(Side::WHITE) & bb) {
                if (p.kings() & bb)
                        sstr << Token::UPPER[Side::WHITE];              // white king
                else
                        sstr << Token::LOWER[Side::WHITE];              // white man
        } else
                sstr << Token::EMPTY;                                   // empty square

        return sstr.str();
}

template<typename Board>
bool is_valid_square(size_t sq)
{
        return sq < Board::SQUARE_RANGE;
}
