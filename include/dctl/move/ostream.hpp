#pragma once
#include <dctl/move/move.hpp>   // Move
#include <dctl/move/manip.hpp>  // notation, pushsep, jumpsep
#include <dctl/move/traits.hpp> // notation, pushsep, jumpsep
#include <dctl/rule_traits.hpp> // jumpsep, pushsep
#include <cassert>              // assert
#include <iosfwd>               // ostream
#include <sstream>              // stringstream

namespace dctl {
namespace move {

template<class Rules, class Board>
auto getnotation(std::ios_base& str)
{
        auto const iword = gnotation(str);
        return xstd::to_underlying_type(iword) ? iword : traits::xnotation<Rules, Board>::value;
}

template<class Rules>
auto getpushsep(std::ios_base& str)
{
        if (auto const iword = pushsep(str))
                return iword;
        return pushsep_v<Rules>;
}

template<class Rules>
auto getjumpsep(std::ios_base& str)
{
        if (auto const iword = jumpsep(str))
                return iword;
        return jumpsep_v<Rules>;
}

template<class Move>
auto separator(std::ostream& ostr, Move const& m)
{
        using Rules = typename Move::rules_type;
        return (m.is_jump() ? getjumpsep<Rules>(ostr) : getpushsep<Rules>(ostr));
}

template<class Move>
auto& print_algebraic(std::ostream& ostr, Move const& m)
{
        using Board = typename Move::board_type;
        ostr << Board::algebraic_from_bit(m.from());
        ostr << separator(ostr, m);
        ostr << Board::algebraic_from_bit(m.dest());
        return ostr;
}

template<class Move>
auto& print_numeric(std::ostream& ostr, Move const& m)
{
        using Board = typename Move::board_type;
        ostr << Board::numeric_from_bit(m.from());
        ostr << separator(ostr, m);
        ostr << Board::numeric_from_bit(m.dest());
        return ostr;
}

template<class Move>
auto str_algebraic(Move const& m)
{
        std::stringstream sstr;
        print_algebraic(sstr, m);
        return sstr.str();
}

template<class Move>
auto str_numeric(Move const& m)
{
        std::stringstream sstr;
        print_numeric(sstr, m);
        return sstr.str();
}

}       // namespace move

template<class CharT, class Traits, class Rules, class Board>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, Move<Rules, Board> const& m)
{
        switch(move::getnotation<Rules, Board>(ostr)) {
        case Notation::algebraic : return move::print_algebraic(ostr, m);
        case Notation::numeric   : return move::print_numeric(ostr, m);
        }
}

}       // namespace dctl
