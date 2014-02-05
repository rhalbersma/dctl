#pragma once
#include <cassert>                      // assert
#include <iosfwd>                       // ostream
#include <sstream>                      // stringstream
#include <dctl/move/move.hpp>           // Move
#include <dctl/move/manip.hpp>          // notation, pushsep, jumpsep
#include <dctl/move/trait.hpp>          // notation, pushsep, jumpsep

namespace dctl {
namespace move {

template<class Rules, class Board>
auto getnotation(std::ostream& ostr)
{
        auto index = ostr.iword(manip::notation());
        if (!index)
                index = trait::notation<Rules, Board>::value;
        return index;
}

template<class Rules>
auto getpushsep(std::ostream& ostr)
{
        auto index = static_cast<char>(ostr.iword(manip::pushsep()));
        if (!index)
                index = trait::pushsep<Rules>::value;
        return index;
}

template<class Rules>
auto getjumpsep(std::ostream& ostr)
{
        auto index = static_cast<char>(ostr.iword(manip::jumpsep()));
        if (!index)
                index = trait::jumpsep<Rules>::value;
        return index;
}

template<class Move>
auto& print_algebraic(std::ostream& ostr, Move const& m)
{
        using Rules = typename Move::rules_type;
        using Board = typename Move::board_type;
        ostr << Board::algebraic_from_bit(m.from());
        ostr << (m.is_jump() ? getjumpsep<Rules>(ostr) : getpushsep<Rules>(ostr));
        ostr << Board::algebraic_from_bit(m.dest());
        return ostr;
}

template<class Move>
auto& print_numeric(std::ostream& ostr, Move const& m)
{
        using Rules = typename Move::rules_type;
        using Board = typename Move::board_type;
        ostr << Board::numeric_from_bit(m.from());
        ostr << (m.is_jump() ? getjumpsep<Rules>(ostr) : getpushsep<Rules>(ostr));
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
        case algebraic:
                return move::print_algebraic(ostr, m);
        case numeric:
                return move::print_numeric(ostr, m);
        default:
                assert(false);
                return ostr;
        }
}

}       // namespace dctl
