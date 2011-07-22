#include <cctype>
#include "../board/Board.h"
#include "../board/Layout.h"
#include "Protocol.h"
#include "../protocol/damexchange/Protocol.h"

namespace dctl {
namespace string {

template<typename Token>
bool read_color(char c)
{
	switch(c) {
	case Token::BLACK:
		return Side::BLACK;
	case Token::WHITE:
		return Side::WHITE;
        default:
                assert(false);
                return false;
	}
}

template<typename Token>
char write_color(bool b)
{
	return Token::COLOR[b];
}

template<typename Board, typename Token>
struct read<Board, FEN_tag, Token>
{
        Position<Board> operator()(const std::string& s)
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
                sstr >> ch;
                p_side = read_color<Token>(ch);
                size_t sq;

                for (sstr >> ch; sstr; sstr >> ch) {
                        switch(ch) {
                        case Token::COLON:
                                sstr >> ch;
                                setup_color = read_color<Token>(ch);
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
			                assert(Board::is_valid(sq - 1)); 
                                        auto b = Board::square2bit(sq - 1);     // convert square to bit
			                auto bb = BitBoard(1) << b;             // create bitboard
                                        p_pieces[setup_color] ^= bb;
                                        if (setup_kings)
                                                p_kings ^= bb;
                                }
                                setup_kings = false;
                        }
                }
                return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);
        }
};

template<typename Token>
struct write<FEN_tag, Token>
{
        template<typename Board>
        std::string operator()(const Position<Board>& p) const
        {
                std::stringstream sstr;
	        sstr << Token::QUOTE;					        // opening quotes
	        sstr << Token::COLOR[p.to_move()];				// side to move

                for (auto i = 0; i < 2; ++i) {
		        auto c = i != 0;
		        if (p.pieces(c)) {
			        sstr << Token::COLON;                           // colon
			        sstr << Token::COLOR[c];                        // color tag
		        }
		        for (auto bb = p.pieces(c); bb; bit::clear_first(bb)) {
			        if (p.kings() & bit::get_first(bb))
				        sstr << Token::KING;			// king tag
                                auto b = bit::find_first(bb);                   // bit index                        
			        sstr << Board::bit2square(b) + 1;	        // square number
			        if (bit::is_multiple(bb))                       // still pieces remaining
				        sstr << Token::COMMA;			// comma separator
		        }
	        }
	        sstr << Token::QUOTE;						// closing quotes
	        return sstr.str();
        }
};

template<typename Board, typename Token>
struct read<Board, protocol::damexchange::version, Token>
{
        Position<Board> operator()(const std::string& s)
        {
	        assert(s.length() == Board::SIZE + 1);

	        BitBoard p_pieces[2] = {0, 0};
	        BitBoard p_kings = 0;
	        bool p_side = Side::BLACK;

	        std::stringstream sstr(s);
	        char ch;
	        sstr >> ch;
                p_side = read_color<Token>(ch);

                for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
                        auto b = Board::square2bit(sq);         // convert square to bit
		        auto bb = BitBoard(1) << b;             // create bitboard
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
};

template<typename Token>
struct write<protocol::damexchange::version, Token>
{
        template<typename Board>
        std::string operator()(const Position<Board>& p) const
        {
	        std::stringstream sstr;
	        sstr << write_color<Token>(p.to_move());		// side to move
	        for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
		        auto b = Board::square2bit(sq);                 // convert square to bit
		        sstr << bitContent<Board, Token>()(p, b);       // bit content
	        }
	        return sstr.str();
        }
};

}       // namespace string
}       // namespace dctl
