#include <functional>
#include <iomanip>
#include <sstream>
#include "Board.h"

namespace dctl {
namespace board {

static const char WHITE_SPACE = ' ';

// write board square layout as an ASCII diagram
struct squares {};

template<typename Board>
struct Diagram<Board, squares>
{
        const std::string operator()() const
        {
                return Diagram<Board, squares>()(std::bind(std::plus<int>(), std::placeholders::_1, 1));
        }

        template<typename Functor>
        const std::string operator()(Functor f) const
        {
	        std::stringstream sstr;

                for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
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

// write board bit layout as an ASCII diagram
struct bits {};

template<typename Board>
struct Diagram<Board, bits> 
{
        const std::string operator()() const
        {
                return Diagram<Board, bits>()(std::bind(std::plus<size_t>(), std::placeholders::_1, 0));
        }

        template<typename Functor>
        const std::string operator()(Functor f) const
        {
	        return Diagram<Board, squares>()(std::bind(f, std::bind(Board::square2bit, std::placeholders::_1)));
        }
};

template<typename Board>
bool is_end_row(int sq)
{
        const auto r = sq % Board::ExternalGrid::MODULO;                // sq = MODULO * q + r 
        const auto end_RE = r == Board::ExternalGrid::EDGE_RE;          // right of even rows
        const auto end_RO = r == Board::ExternalGrid::EDGE_RO;          // right of odd rows

        return end_RE || end_RO;
}

template<typename Board>
bool is_indent_row(int sq)
{
        const auto r = sq % Board::ExternalGrid::MODULO;                // sq = MODULO * q + r 
        const auto indent_LE = r == Board::ExternalGrid::EDGE_LE;       // left of even rows
        const auto indent_LO = r == Board::ExternalGrid::EDGE_LO;       // left of odd rows

        return Board::PARITY? indent_LO : indent_LE;
}

}       // namespace board
}       // namespace dctl
