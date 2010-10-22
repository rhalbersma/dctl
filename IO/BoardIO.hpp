#include "Token.h"
#include "../Board/Board.h"
#include <iomanip>
#include <sstream>

// write board square layout as an ASCII board
template<typename Board>
std::string write_square_layout<Board>::operator()(void) const
{
        return write_square_layout<Board>()(PlusOne());
}

template<typename Board> template<typename Functor>
std::string write_square_layout<Board>::operator()(Functor f) const
{
	std::stringstream sstr;

        for (size_t sq = 0; sq < Board::NUM_SQUARES; ++sq) {
                if (is_indent_row<Board>()(sq))
                        sstr << std::setw(2) << WHITE_SPACE;    // start of an indented row

                sstr << std::setw(2) << f(sq);                  // write square content 

                if (is_end_row<Board>()(sq))
                        sstr << std::endl;                      // start of a new row
                else
                        sstr << std::setw(2) << WHITE_SPACE;    // space between squares
        }
        return sstr.str();
}

// write board bit layout as an ASCII board
template<typename Board>
std::string write_bit_layout<Board>::operator()(void) const
{
        return write_bit_layout<Board>()(Identity());
}

template<typename Board> template<typename Functor>
std::string write_bit_layout<Board>::operator()(Functor f) const
{
	std::stringstream sstr;
        size_t b;

        for (size_t sq = 0; sq < Board::NUM_SQUARES; ++sq) {
                if (is_indent_row<Board>()(sq))
                        sstr << std::setw(2) << WHITE_SPACE;    // start of an indented row

                b = Board::TABLE_SQUARE2BIT[sq];                // convert square to bit
                sstr << std::setw(2) << f(b);                   // write bit content 

                if (is_end_row<Board>()(sq))
                        sstr << std::endl;                      // start of a new row
                else
                        sstr << std::setw(2) << WHITE_SPACE;    // space between squares
        }
        return sstr.str();
}

template<typename Board>
bool is_end_row<Board>::operator()(size_t sq) const
{
        const size_t R = sq % Board::SQUARE_MODULO;             // sq = SQUARE_MODULO * Q + R 
        const bool END_RE = R == Board::SQUARE_RE;              // right of even rows
        const bool END_RO = R == Board::SQUARE_RO;              // right of odd rows

        return END_RE || END_RO;
}

template<typename Board>
bool is_indent_row<Board>::operator()(size_t sq) const
{
        const size_t R = sq % Board::SQUARE_MODULO;             // sq = SQUARE_MODULO * Q + R 
        const bool BEGIN_LE = R == Board::SQUARE_LE;            // left of even rows
        const bool BEGIN_LO = R == Board::SQUARE_LO;            // left of odd rows

        return Board::PARITY? BEGIN_LO : BEGIN_LE;
}

struct PlusOne: public std::unary_function<size_t, size_t>
{
        size_t operator()(size_t sq) const
        {
                return sq + 1;
        }
};

struct Identity: public std::unary_function<size_t, size_t>
{
        size_t operator()(size_t b) const
        {
                return b;
        }
};
