#include <functional>
#include <iomanip>
#include <sstream>
#include "../node/Position.h"
#include "../node/Material.h"
#include "../node/Side.h"
#include "../bit/Bit.h"
#include "../utils/IntegerTypes.h"

namespace dctl {
namespace setup {

static const char WHITE_SPACE = ' ';

struct squares {};

// partial specialization to write square numbers in diagram layout
template<typename Board>
struct diagram<Board, squares>
{
        std::string operator()() const
        {
                return diagram<Board, squares>()(std::bind(std::plus<int>(), std::placeholders::_1, 1));
        }

        template<typename Functor>
        std::string operator()(Functor f) const
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

struct bits {};

// partial specialization to write bit numbers in diagram layout
template<typename Board>
struct diagram<Board, bits> 
{
        std::string operator()() const
        {
                return diagram<Board, bits>()(std::bind(std::plus<int>(), std::placeholders::_1, 0));
        }

        template<typename Functor>
        std::string operator()(Functor f) const
        {
	        return diagram<Board, squares>()(std::bind(f, std::bind(Board::square2bit, std::placeholders::_1)));
        }
};

// partial specialization to write position content in diagram layout
template<typename Protocol, typename Setup> template<typename Board>
std::string diagram<Protocol, Setup>::operator()(const Position<Board>& p) const
{
        return diagram<Board, bits>()(std::bind(bit_content<Setup>, p.material(), std::placeholders::_1));
}

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

template<typename Setup>
std::string bit_content(const Material& p, int b)
{
        const BitBoard bb = BitBoard(1) << b;

        std::stringstream sstr;
        if (p.pieces(Side::BLACK) & bb) {
                if (p.kings() & bb)
			sstr << Setup::UPPER[Side::BLACK];      // black king
                else
                        sstr << Setup::LOWER[Side::BLACK];      // black man
        } else if (p.pieces(Side::WHITE) & bb) {
                if (p.kings() & bb)
                        sstr << Setup::UPPER[Side::WHITE];      // white king
                else
                        sstr << Setup::LOWER[Side::WHITE];      // white man
        } else
                sstr << Setup::EMPTY;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
