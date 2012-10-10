#include <functional>   // std::bind, std::placeholders
#include <iomanip>      // std::setw
#include <sstream>      // std::stringstream
#include "Content.h"
#include "Numbers.h"
#include "../node/Position.h"
#include "../node/Side.h"
#include "../utils/IntegerTypes.h"

namespace dctl {
namespace setup {

// position content in diagram layout
template<typename Protocol, typename Content> template<typename Board>
std::string diagram<Protocol, Content>::operator()(const Position<Board>& p) const
{
        return diagram<Board, bits>()(std::bind(content<Content>, p.material(), std::placeholders::_1));
}

// partial specialization to write bit numbers in diagram layout
template<typename Board>
class diagram<Board, bits> 
{
public:
        // the board bit numbers (starting at 0)
        std::string operator()() const
        {
                return diagram<Board, bits>()(std::bind(std::plus<int>(), std::placeholders::_1, 0));
        }

        // parameterized board bit content
        template<typename Functor>
        std::string operator()(Functor f) const
        {
	        return diagram<Board, squares>()(std::bind(f, std::bind(Board::square2bit, std::placeholders::_1)));
        }
};

// partial specialization to write square numbers in diagram layout
template<typename Board>
class diagram<Board, squares>
{
public:
        // the board square numbers (starting at 1)
        std::string operator()() const
        {
                return diagram<Board, squares>()(std::bind(std::plus<int>(), std::placeholders::_1, 1));
        }
        
        // parameterized board square content
        template<typename Functor>
        std::string operator()(Functor f) const
        {
	        std::stringstream sstr;

                for (auto sq = Board::begin(); sq != Board::end(); ++sq) {
                        if (is_indent_row(sq))
                                sstr << std::setw(2) << WHITE_SPACE;    // start of an indented row

                        sstr << std::setw(2) << f(sq);                  // write square content 

                        if (is_end_row(sq))
                                sstr << std::endl;                      // start of a new row
                        else
                                sstr << std::setw(2) << WHITE_SPACE;    // space between squares
                }
                return sstr.str();
        }

private:
        static bool is_end_row(int sq)
        {
                const auto r = sq % Board::ExternalGrid::MODULO;                // sq = MODULO * q + r 
                const auto end_RE = r == Board::ExternalGrid::EDGE_RE;          // right of even rows
                const auto end_RO = r == Board::ExternalGrid::EDGE_RO;          // right of odd rows

                return end_RE || end_RO;
        }

        static bool is_indent_row(int sq)
        {
                const auto r = sq % Board::ExternalGrid::MODULO;                // sq = MODULO * q + r 
                const auto indent_LE = r == Board::ExternalGrid::EDGE_LE;       // left of even rows
                const auto indent_LO = r == Board::ExternalGrid::EDGE_LO;       // left of odd rows

                return Board::PARITY? indent_LO : indent_LE;
        }

        static const char WHITE_SPACE = ' ';
};

}       // namespace setup
}       // namespace dctl
