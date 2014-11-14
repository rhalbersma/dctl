#pragma once
#include <dctl/setup/content.hpp>
#include <dctl/setup/diagram_fwd.hpp>
#include <dctl/setup/numbers.hpp>
#include <dctl/setup/i_token.hpp>
#include <dctl/position/color.hpp>
#include <dctl/type_traits.hpp>
#include <iomanip>                      // setw
#include <sstream>                      // stringstream
#include <string>                       // string

namespace dctl {
namespace setup {

// partial specialization definition to write square numbers in diagram layout
template<class Board>
struct diagram<Board, squares>
{
public:
        // the board square numbers (starting at 1)
        std::string operator()() const
        {
                return diagram<Board, squares>{}([](auto n){ return n + 1; });
        }

        // parameterized board square content
        template<class Functor>
        std::string operator()(Functor f) const
        {
                std::stringstream sstr;

                for (auto&& sq : Board::squares()) {
                        if (is_indent_row(sq))
                                sstr << std::setw(2) << WHITE_SPACE;    // start of an indented row

                        sstr << std::setw(2) << f(sq);                  // write square content

                        if (is_end_row(sq))
                                sstr << '\n';                           // start of a new row
                        else
                                sstr << std::setw(2) << WHITE_SPACE;    // space between squares
                }
                sstr << '\n';
                return sstr.str();
        }

private:
        static bool is_end_row(int sq)
        {
                auto const r = sq % Board::modulo();         // sq = modulo * q + r
                auto const end_RE = r == Board::edge_re();   // right of even rows
                auto const end_RO = r == Board::edge_ro();   // right of odd rows

                return end_RE || end_RO;
        }

        static bool is_indent_row(int sq)
        {
                auto const r = sq % Board::modulo();                 // sq = modulo * q + r
                auto const indent_LE = r == Board::edge_le();        // left of even rows
                auto const indent_LO = r == Board::edge_lo();        // left of odd rows

                return Board::ul_parity() ? indent_LO : indent_LE;
        }

        static const auto WHITE_SPACE = ' ';
};

// partial specialization definition to write bit numbers in diagram layout
template<class Board>
struct diagram<Board, bits>
{
public:
        // the board bit numbers (starting at 0)
        std::string operator()() const
        {
                return diagram<Board, bits>{}([](auto n){ return n; });
        }

        // parameterized board bit content
        template<class Functor>
        std::string operator()(Functor f) const
        {
                return diagram<Board, squares>{}([=](auto n){ return f(Board::bit_from_square(static_cast<int>(n))); });
        }
};

// primary template definition
template
<
        class Protocol,
        class Content
>
struct diagram
{
public:
        // position content in diagram layout
        template<class Position>
        std::string operator()(Position const& p) const
        {
                return diagram<board_type_t<Position>, bits>{}([&](auto n){ return content<Content, Position>(p, static_cast<std::size_t>(n)); });
        }
};

}       // namespace setup
}       // namespace dctl
