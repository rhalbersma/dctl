#pragma once
#include <functional>                   // std::bind, std::placeholders
#include <iomanip>                      // std::setw
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Content.hpp"
#include "Numbers.hpp"
#include "Token.hpp"
#include "../node/Position.hpp"
#include "../node/Side.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace setup {

template
<
        typename Protocol, 
        typename Content = typename Token<Protocol>::type
>
class diagram
{
public:
        // position content in diagram layout
        template<typename Board> 
        std::string operator()(const Position<Board>& p) const
        {
                return diagram<Board, bits>()(std::bind(content<Content>, p.material(), std::placeholders::_1));
        }
};

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
                                sstr << "\n";                           // start of a new row
                        else
                                sstr << std::setw(2) << WHITE_SPACE;    // space between squares
                }
                sstr << "\n";
                return sstr.str();
        }

private:
        static bool is_end_row(int sq)
        {
                const auto r = sq % Board::ExternalGrid::modulo;                // sq = modulo * q + r 
                const auto end_RE = r == Board::ExternalGrid::edge_re;          // right of even rows
                const auto end_RO = r == Board::ExternalGrid::edge_ro;          // right of odd rows

                return end_RE || end_RO;
        }

        static bool is_indent_row(int sq)
        {
                const auto r = sq % Board::ExternalGrid::modulo;                // sq = modulo * q + r 
                const auto indent_LE = r == Board::ExternalGrid::edge_le;       // left of even rows
                const auto indent_LO = r == Board::ExternalGrid::edge_lo;       // left of odd rows

                return Board::parity? indent_LO : indent_LE;
        }

        BOOST_STATIC_CONSTANT(auto, WHITE_SPACE = ' ');
};


}       // namespace setup
}       // namespace dctl
