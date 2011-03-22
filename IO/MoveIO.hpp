#include "Token.h"
#include "../Position/Position.h"
#include "../Position/Pieces.h"
#include "../Variants/Rules.h"
#include <iomanip>
#include <sstream>

// move tokens
template<typename> struct MoveToken		{ typedef MoveTokenDouble<'-', 'x'> TYPE; };
template<> struct MoveToken<Variants::Russian>  { typedef MoveTokenDouble<'-', ':'> TYPE; };
template<> struct MoveToken<Variants::Czech>	{ typedef MoveTokenDouble<'-', ':'> TYPE; };

template<typename Rules> template<typename Board>
std::string write_move_string<Rules>::operator()(const Position<Board>& p, const Pieces& m)
{
        typedef typename MoveToken<Rules>::TYPE Token;

        std::stringstream sstr;
        sstr << std::setw(2) << std::right << Board::TABLE_BIT2SQUARE[Bit::scan_forward(Move::from_sq(p, m))] + 1;
        sstr << (Move::is_capture(p, m)? Token::CAPTURE : Token::MOVE);
        sstr << std::setw(2) << std::left  << Board::TABLE_BIT2SQUARE[Bit::scan_forward(Move::dest_sq(p, m))] + 1;
        return sstr.str();
}
