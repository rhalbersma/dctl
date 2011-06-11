#include <functional>
#include <iomanip>
#include <sstream>

namespace geometry {
namespace layout {

static const char WHITE_SPACE = ' ';

// write board square layout as an ASCII board
struct Square_tag {};

template<typename Board>
struct write<Board, Square_tag>
{
        std::string operator()(void) const
        {
                return write<Board, Square_tag>()(std::bind(std::plus<int>(), std::placeholders::_1, 1));
        }

        template<typename Functor>
        std::string operator()(Functor f) const
        {
	        std::stringstream sstr;

                for (size_t sq = 0; is_valid<Board>(sq); ++sq) {
                        if (is_indent_row<Board>(sq))
                                sstr << std::setw(2) << WHITE_SPACE;    // start of an indented row

                        sstr << std::setw(2) << f(sq);                  // write square content 

                        if (is_end_row<Board>(sq))
                                sstr << std::endl;                      // start of a new row
                        else
                                sstr << std::setw(2) << WHITE_SPACE;    // space between squares
                }
                return sstr.str();
        }
};

// write board bit layout as an ASCII board
struct Bit_tag {};

template<typename Board>
struct write<Board, Bit_tag> 
{
        std::string operator()(void) const
        {
                return write<Board, Bit_tag>()(std::bind(std::plus<size_t>(), std::placeholders::_1, 0));
        }

        template<typename Functor>
        std::string operator()(Functor f) const
        {
	        return write<Board, Square_tag>()(std::bind(f, std::bind(square2bit<Board>, std::placeholders::_1)));
        }
};

template<typename Board>
size_t square2bit(size_t sq)
{
        return Board::TABLE_SQUARE2BIT[sq];
}

template<typename Board>
bool is_valid(size_t sq)
{
        return sq < Board::SIZE;
}

template<typename Board>
bool is_end_row(size_t sq)
{
        const size_t r = sq % Board::MODULO;                    // sq = MODULO * q + r 
        const bool end_RE = r == Board::EDGE_RE;                // right of even rows
        const bool end_RO = r == Board::EDGE_RO;                // right of odd rows

        return end_RE || end_RO;
}

template<typename Board>
bool is_indent_row(size_t sq)
{
        const size_t r = sq % Board::MODULO;                    // sq = MODULO * q + r 
        const bool indent_LE = r == Board::EDGE_LE;             // left of even rows
        const bool indent_LO = r == Board::EDGE_LO;             // left of odd rows

        return Board::PARITY? indent_LO : indent_LE;
}

}       // namespace layout
}       // namespace geometry
