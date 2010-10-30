#include "Token.h"
#include <iomanip>
#include <sstream>

// write board square layout as an ASCII board
template<typename T>
std::string write_square_layout<T>::operator()(void) const
{
        return write_square_layout<T>()(PlusOne());
}

template<typename T> template<typename Functor>
std::string write_square_layout<T>::operator()(Functor f) const
{
	std::stringstream sstr;

        for (size_t sq = 0; sq < T::NUM_SQUARES; ++sq) {
                if (is_indent_row<T>(sq))
                        sstr << std::setw(2) << WHITE_SPACE;    // start of an indented row

                sstr << std::setw(2) << f(sq);                  // write square content 

                if (is_end_row<T>(sq))
                        sstr << std::endl;                      // start of a new row
                else
                        sstr << std::setw(2) << WHITE_SPACE;    // space between squares
        }
        return sstr.str();
}

// write board bit layout as an ASCII board
template<typename T>
std::string write_bit_layout<T>::operator()(void) const
{
        return write_bit_layout<T>()(Identity());
}

template<typename T> template<typename Functor>
std::string write_bit_layout<T>::operator()(Functor f) const
{
	std::stringstream sstr;
        size_t b;

        for (size_t sq = 0; sq < T::NUM_SQUARES; ++sq) {
                if (is_indent_row<T>(sq))
                        sstr << std::setw(2) << WHITE_SPACE;    // start of an indented row

                b = T::TABLE_SQUARE2BIT[sq];                    // convert square to bit
                sstr << std::setw(2) << f(b);                   // write bit content 

                if (is_end_row<T>(sq))
                        sstr << std::endl;                      // start of a new row
                else
                        sstr << std::setw(2) << WHITE_SPACE;    // space between squares
        }
        return sstr.str();
}

template<typename T> inline
bool is_end_row(size_t sq)
{
        const size_t r = sq % T::SQUARE_MODULO;                 // sq = SQUARE_MODULO * q + r 
        const bool end_RE = r == T::SQUARE_RE;                  // right of even rows
        const bool end_RO = r == T::SQUARE_RO;                  // right of odd rows

        return end_RE || end_RO;
}

template<typename T>
bool is_indent_row(size_t sq)
{
        const size_t r = sq % T::SQUARE_MODULO;                 // sq = SQUARE_MODULO * q + r 
        const bool begin_LE = r == T::SQUARE_LE;                // left of even rows
        const bool begin_LO = r == T::SQUARE_LO;                // left of odd rows

        return T::COLORING? begin_LO : begin_LE;
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
