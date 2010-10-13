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

        bool setup_kings = false;
        bool setup_color = p_side;

        std::stringstream sstr(s);
        char ch;
        size_t sq;
        BitBoard b;

	sstr >> ch;
	switch(ch) {
	case Token::BLACK:
		p_side = Side::BLACK;			        // black to move
		break;
	case Token::WHITE:
		p_side = Side::WHITE;			        // white to move
		break;
        default:
                assert(false);
	}

        for(sstr >> ch; sstr; sstr >> ch) {
                switch(ch) {
                case Token::COLON:
                        sstr >> ch;
                        switch(ch) {
                        case Token::BLACK:                      // setup black pieces
                                setup_color = Side::BLACK;
                                break;
                        case Token::WHITE:                      // setup white pieces
                                setup_color = Side::WHITE;
                                break;
                        default:
                                assert(false);
                        }
                        break;
                case Token::KING:                               // setup kings
                        setup_kings = true;
                        break;
                case Token::COMMA:
                        break;
                default:
                        if (isdigit(ch)) {
                                sstr.putback(ch);
                                sstr >> sq;                     // read square
			        assert(is_valid_square<Board>(sq));
			        b = BitBoard(1) << Board::TABLE_SQUARE2BIT[sq - 1];
                                p_pieces[setup_color] ^= b;
                                if (setup_kings)
                                        p_kings ^= b;
                        }
                        setup_kings = false;
                }
        }
        return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);
}

template<typename Token> template<typename Board>
std::string write_position_string<FEN, Token>::operator()(const Position<Board>& p)
{
        std::stringstream sstr;
	bool c;

	sstr << Token::QUOTE;								// opening quotes
	sstr << Token::COLOR[p.to_move()];						// side to move
	for (size_t i = 0; i < 2; ++i) {
		c = i != 0;
		if (p.pieces(c)) {
			sstr << Token::COLON;                                           // colon
			sstr << Token::COLOR[c];					// color tag
		}
		for (BitBoard b = p.pieces(c); b; Bit::clear_lowest(b)) {
			if (p.kings() & Bit::get_lowest(b))
				sstr << Token::KING;					// king tag
			sstr << Board::TABLE_BIT2SQUARE[Bit::scan_forward(b)] + 1;	// square number
			if (Bit::is_multiple(b))                                        // still pieces remaining
				sstr << Token::COMMA;					// comma separator
		}
	}
	sstr << Token::QUOTE;								// closing quotes
	return sstr.str();
}

//+----------------------------------------------------------------------------+

template<typename Board, typename Token>
Position<Board> read_position_string<DXP, Board, Token>::operator()(const std::string& s)
{
	assert(s.length() == Board::NUM_SQUARES + 1);

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

	BitBoard b;
	for(size_t i = 0; i < Board::NUM_SQUARES; ++i) {
		b = BitBoard(1) << Board::TABLE_SQUARE2BIT[i];
		sstr >> ch;
		switch(toupper(ch)) {
		case Token::BLACK:			// black piece
			p_pieces[Side::BLACK] ^= b;
			break;
		case Token::WHITE:			// white piece
			p_pieces[Side::WHITE] ^= b;
			break;
                case Token::EMPTY:
                        break;
                default:
                        assert(false);
		}
                if (islower(ch))
                        p_kings ^= b;                   // king
	}
	return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);
}

template<typename Token> template<typename Board>
std::string write_position_string<DXP, Token>::operator()(const Position<Board>& p)
{
	std::stringstream sstr;
	BitBoard b;

	sstr << Token::COLOR[p.to_move()];				// side to move
	for (size_t i = 0; i < Board::NUM_SQUARES; ++i) {
		b = BitBoard(1) << Board::TABLE_SQUARE2BIT[i];          // convert square to bit
		if (p.pieces(Side::BLACK) & b) {
			if (p.kings() & b)
                                sstr << Token::UPPER[Side::BLACK];	// black king
			else
                                sstr << Token::LOWER[Side::BLACK];	// black man
		} else if (p.pieces(Side::WHITE) & b) {
			if (p.kings() & b)
                                sstr << Token::UPPER[Side::WHITE];	// white king
			else
                                sstr << Token::LOWER[Side::WHITE];      // white man
		} else
			sstr << Token::EMPTY;			        // empty square
	}
	return sstr.str();
}

//+----------------------------------------------------------------------------+

template<typename Protocol, typename Token> template<typename Board>
std::string write_position_stream<Protocol, Token>::operator()(const Position<Board>& p)
{
	std::stringstream sstr;
        size_t b1, q1, r1, c1, r2, b2;
        BitBoard b;

        for (size_t i = 0; i < Board::NUM_SQUARES; ++i) {
                b1 = Board::TABLE_SQUARE2BIT[i];                                // convert square to bit
                q1 = b1 / Board::GHOST_MODULO;                                  // b1 = GHOST_MODULO * q1 + r1
                r1 = b1 % Board::GHOST_MODULO;
                c1 = r1 > Board::GHOST_RE;                                      // compare r1 to end of first row
                b2 = b1 + Board::GHOST_MODULO - 1;
                r2 = b2 % Board::GHOST_MODULO;                                  // r2 == (r1 - 1) mod GHOST_MODULO

                if ((r2 == (c1? Board::GHOST_LO : Board::GHOST_LE)) && (c1 ^ Board::PARITY))
                        sstr << std::setw(2) << WHITE_SPACE;                    // start of an indented row

                sstr << std::setw(2);
                b = BitBoard(1) << b1;
                if (p.pieces(Side::BLACK) & b) {
                        if (p.kings() & b)
				sstr << Token::UPPER[Side::BLACK];              // black king
                        else
                                sstr << Token::LOWER[Side::BLACK];              // black man
                } else if (p.pieces(Side::WHITE) & b) {
                        if (p.kings() & b)
                                sstr << Token::UPPER[Side::WHITE];              // white king
                        else
                                sstr << Token::LOWER[Side::WHITE];              // white man
                } else
                        sstr << Token::EMPTY;                                   // empty square

                if (r1 == (c1? Board::GHOST_RO : Board::GHOST_RE) - 1)
                        sstr << std::endl;                                      // start of a new row
                else
                        sstr << std::setw(2) << WHITE_SPACE;                    // space between squares
        }
        return sstr.str();
}
