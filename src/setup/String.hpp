#include <cctype>
#include "Diagram.h"

namespace dctl {
namespace setup {

template<typename Setup>
bool read_color(char c)
{
	switch(c) {
	case Setup::BLACK:
		return Side::BLACK;
	case Setup::WHITE:
		return Side::WHITE;
        default:
                assert(false);
                return false;
	}
}

template<typename Setup>
char write_color(bool color)
{
	return Setup::COLOR[color];
}

template<typename Board, typename Setup>
struct read<Board, pdn::protocol, Setup>
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
                std::size_t sq;

                for (sstr >> ch; sstr; sstr >> ch) {
                        switch(ch) {
                        case Setup::BLACK:
                                // falling through
                        case Setup::WHITE:
                                p_side = read_color<Setup>(ch);
                                break;                                
                        case Setup::COLON:
                                sstr >> ch;
                                setup_color = read_color<Setup>(ch);
                                break;
                        case Setup::KING:                                       // setup kings
                                setup_kings = true;
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
                                break;
                        }
                }
                return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);
        }
};

template<typename Setup>
struct write<pdn::protocol, Setup>
{
        template<typename Board>
        std::string operator()(const Position<Board>& p) const
        {
                std::stringstream sstr;
	        sstr << Setup::QUOTE;					        // opening quotes
	        sstr << write_color<Setup>(p.active_color());			// side to move

                for (auto i = 0; i < 2; ++i) {
		        auto c = i != 0;
		        if (p.pieces(c)) {
			        sstr << Setup::COLON;                           // colon
			        sstr << Setup::COLOR[c];                        // color tag
		        }
		        for (auto bb = p.pieces(c); bb; bit::clear_first(bb)) {
			        if (p.kings() & bit::get_first(bb))
				        sstr << Setup::KING;			// king tag
                                auto b = bit::find_first(bb);                   // bit index                        
			        sstr << Board::bit2square(b) + 1;	        // square number
			        if (bit::is_multiple(bb))                       // still pieces remaining
				        sstr << Setup::COMMA;			// comma separator
		        }
	        }
	        sstr << Setup::QUOTE;						// closing quotes
	        return sstr.str();
        }
};

template<typename Board, typename Setup>
struct read<Board, dxp::protocol, Setup>
{
        Position<Board> operator()(const std::string& s) const
        {
	        assert(s.length() == Board::ExternalGrid::SIZE + 1);

	        BitBoard p_pieces[2] = {0, 0};
	        BitBoard p_kings = 0;
	        bool p_side = Side::BLACK;

	        std::stringstream sstr(s);
	        char ch;
	        sstr >> ch;
                p_side = read_color<Setup>(ch);

                for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
                        auto b = Board::square2bit(sq);         // convert square to bit
		        auto bb = BitBoard(1) << b;             // create bitboard
		        sstr >> ch;
		        switch(toupper(ch)) {
		        case Setup::BLACK:			
			        p_pieces[Side::BLACK] ^= bb;    // black piece
			        break;
		        case Setup::WHITE:			
			        p_pieces[Side::WHITE] ^= bb;    // white piece
			        break;
                        case Setup::EMPTY:
                                break;
                        default:
                                assert(false);
                                break;
		        }
                        if (isupper(ch))
                                p_kings ^= bb;                  // king
	        }
	        return Position<Board>(p_pieces[Side::BLACK], p_pieces[Side::WHITE], p_kings, p_side);
        }
};

template<typename Setup>
struct write<dxp::protocol, Setup>
{
        template<typename Board>
        std::string operator()(const Position<Board>& p) const
        {
	        std::stringstream sstr;
	        sstr << write_color<Setup>(p.active_color());		// side to move
	        for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
		        auto b = Board::square2bit(sq);                 // convert square to bit
		        sstr << bit_content<Setup>(p.material(), b);    // bit content
	        }
	        return sstr.str();
        }
};

}       // namespace setup
}       // namespace dctl
