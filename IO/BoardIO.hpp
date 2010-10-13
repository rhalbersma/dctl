#include "Token.h"
#include "../Board/Board.h"
#include <iomanip>
#include <sstream>

// write board square layout as an ASCII board
template<typename Board>
std::string write_square_layout<Board>::operator()(void)
{
	std::stringstream sstr;
        size_t b1, q1, r1, c1, r2, b2;

        for (size_t i = 0; i < Board::NUM_SQUARES; ++i) {
                b1 = Board::TABLE_SQUARE2BIT[i];                                // convert square to bit
                q1 = b1 / Board::GHOST_MODULO;                                  // b1 = GHOST_MODULO * q1 + r1
                r1 = b1 % Board::GHOST_MODULO;
                c1 = r1 > Board::GHOST_RE;                                      // compare r1 to end of first row
                b2 = b1 + Board::GHOST_MODULO - 1;
                r2 = b2 % Board::GHOST_MODULO;                                  // r2 == (r1 - 1) mod GHOST_MODULO

                if ((r2 == (c1? Board::GHOST_LO : Board::GHOST_LE)) && (c1 ^ Board::PARITY))
                        sstr << std::setw(2) << WHITE_SPACE;                    // start of an indented row
                sstr << std::setw(2) << (i + 1);                                // square number (starting at 1)

                if (r1 == (c1? Board::GHOST_RO : Board::GHOST_RE) - 1)
                        sstr << std::endl;                                      // start of a new row
                else
                        sstr << std::setw(2) << WHITE_SPACE;                    // space between squares
        }
        return sstr.str();
}

// write board bit layout as an ASCII board
template<typename Board>
std::string write_bit_layout<Board>::operator()(void)
{
	std::stringstream sstr;
        size_t b1, q1, r1, c1, r2, b2;

        for (size_t i = 0; i < Board::NUM_SQUARES; ++i) {
                b1 = Board::TABLE_SQUARE2BIT[i];                                // convert square to bit
                q1 = b1 / Board::GHOST_MODULO;                                  // b1 = GHOST_MODULO * q1 + r1
                r1 = b1 % Board::GHOST_MODULO;
                c1 = r1 > Board::GHOST_RE;                                      // compare r1 to end of first row
                b2 = b1 + Board::GHOST_MODULO - 1;
                r2 = b2 % Board::GHOST_MODULO;                                  // r2 == (r1 - 1) mod GHOST_MODULO

                if ((r2 == (c1? Board::GHOST_LO : Board::GHOST_LE)) && (c1 ^ Board::PARITY))
                        sstr << std::setw(2) << WHITE_SPACE;                    // start of an indented row
                sstr << std::setw(2) << b1;                                     // bit number (starting at 0)

                if (r1 == (c1? Board::GHOST_RO : Board::GHOST_RE) - 1)
                        sstr << std::endl;                                      // start of a new row
                else
                        sstr << std::setw(2) << WHITE_SPACE;                    // space between squares
        }
        return sstr.str();
}
