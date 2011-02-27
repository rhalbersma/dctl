#include "Token.h"
#include "../Position/Position.h"
#include "../Position/Pieces.h"
#include "../Variants/Rules.h"
#include <iomanip>
#include <sstream>

// move tokens
template<typename> struct MoveToken_            { typedef MoveToken<'-', 'x'> TYPE; };
template<> struct MoveToken_<Variants::Russian>    { typedef MoveToken<'-', ':'> TYPE; };
template<> struct MoveToken_<Variants::Czech>      { typedef MoveToken<'-', ':'> TYPE; };

template<typename Rules> template<typename Board>
std::string write_move_string<Rules>::operator()(const Position<Board>& p, const Pieces& m)
{
        typedef typename MoveToken_<Rules>::TYPE Token;

        const BitBoard from_sq  =  p.pieces( p.to_move()) & m.pieces( p.to_move());
        const BitBoard dest_sq  = ~p.pieces( p.to_move()) & m.pieces( p.to_move());
        const BitBoard captured =  p.pieces(!p.to_move()) & m.pieces(!p.to_move());

        std::stringstream sstr;

        sstr << std::setw(2) << std::right << Board::TABLE_BIT2SQUARE[Bit::scan_forward(from_sq)] + 1;
        sstr << (captured? Token::CAPTURE : Token::MOVE);
        sstr << std::setw(2) << std::left  << Board::TABLE_BIT2SQUARE[Bit::scan_forward(dest_sq)] + 1;

        return sstr.str();
}
