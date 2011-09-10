#include <cctype>
#include "Diagram.h"
#include "Protocols.h"

namespace dctl {
namespace setup {

template<typename Token>
bool read_color(char c)
{
	switch(c) {
	case Token::BLACK:
		return Side::BLACK;
	case Token::WHITE:
		return Side::WHITE;
        default:
                BOOST_ASSERT(false);
                return false;
	}
}

template<typename Token>
char write_color(bool color)
{
	return Token::color[color];
}

template<typename Board, typename Token>
struct read<Board, pdn::protocol, Token>
{
        Position<Board> operator()(const std::string& s) const
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
                int sq;

                for (sstr >> ch; sstr; sstr >> ch) {
                        switch(ch) {
                        case Token::BLACK:
                                // falling through
                        case Token::WHITE:
                                p_side = read_color<Token>(ch);
                                break;                                
                        case Token::COLON:
                                sstr >> ch;
                                setup_color = read_color<Token>(ch);
                                break;
                        case Token::KING:                                       // setup kings
                                setup_kings = true;
                                break;
                        default:
                                if (isdigit(ch)) {
                                        sstr.putback(ch);
                                        sstr >> sq;                             // read square
                                        BOOST_ASSERT(Board::is_valid(sq - 1));
                                        auto b = Board::square2bit(sq - 1);     // convert square to bit
                                        auto bb = BitBoard(1) << b;             // create bitboard
                                        p_pieces[setup_color] ^= bb;
                                        if (setup_kings)
                                                p_kings ^= bb;
                                }
                                setup_kings = false;
                                break;
                        }
                }
                return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);
        }
};

template<typename Token>
struct write<pdn::protocol, Token>
{
        template<typename Board>
        std::string operator()(const Position<Board>& p) const
        {
                std::stringstream sstr;
	        sstr << Token::QUOTE;					        // opening quotes
	        sstr << write_color<Token>(p.active_color());			// side to move

                for (auto i = 0; i < 2; ++i) {
		        auto c = i != 0;
		        if (p.pieces(c)) {
			        sstr << Token::COLON;                           // colon
			        sstr << Token::color[c];                        // color tag
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
	        sstr << Token::QUOTE << "\n";                                   // closing quotes
	        return sstr.str();
        }
};

template<typename Board, typename Token>
struct read<Board, dxp::protocol, Token>
{
        Position<Board> operator()(const std::string& s) const
        {
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
                                BOOST_ASSERT(false);
                                break;
		        }
                        if (isupper(ch))
                                p_kings ^= bb;                  // king
	        }
	        return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);
        }
};

template<typename Token>
struct write<dxp::protocol, Token>
{
        template<typename Board>
        std::string operator()(const Position<Board>& p) const
        {
	        std::stringstream sstr;
	        sstr << write_color<Token>(p.active_color());		// side to move
	        for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
		        auto b = Board::square2bit(sq);                 // convert square to bit
		        sstr << content<Token>(p.material(), b);        // bit content
	        }
                sstr << "\n";
	        return sstr.str();
        }
};

}       // namespace setup
}       // namespace dctl
