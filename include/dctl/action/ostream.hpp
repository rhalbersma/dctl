#pragma once
#include <dctl/action/action.hpp>       // action
#include <dctl/action/manip.hpp>        // notation, pushsep, jumpsep
#include <dctl/action/traits.hpp>       // notation, pushsep, jumpsep
#include <dctl/rule_traits.hpp>         // jumpsep, pushsep
#include <cassert>                      // assert
#include <iosfwd>                       // ostream
#include <sstream>                      // stringstream

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
        return pushsep_or_v<Rules>;
}

template<class Rules>
auto getjumpsep(std::ios_base& str)
{
        if (auto const iword = jumpsep(str))
                return iword;
        return jumpsep_or_v<Rules>;
}

template<class Action>
auto separator(std::ostream& ostr, Action const& m)
{
        using Rules = typename Action::rules_type;
        return (m.is_jump() ? getjumpsep<Rules>(ostr) : getpushsep<Rules>(ostr));
}

template<class Action>
auto& print_algebraic(std::ostream& ostr, Action const& m)
{
        using Board = typename Action::board_type;
        ostr << Board::algebraic_from_bit(m.from());
        ostr << separator(ostr, m);
        ostr << Board::algebraic_from_bit(m.dest());
        return ostr;
}

template<class Action>
auto& print_numeric(std::ostream& ostr, Action const& m)
{
        using Board = typename Action::board_type;
        ostr << Board::numeric_from_bit(m.from());
        ostr << separator(ostr, m);
        ostr << Board::numeric_from_bit(m.dest());
        return ostr;
}

template<class Action>
auto str_algebraic(Action const& m)
{
        std::stringstream sstr;
        print_algebraic(sstr, m);
        return sstr.str();
}

template<class Action>
auto str_numeric(Action const& m)
{
        std::stringstream sstr;
        print_numeric(sstr, m);
        return sstr.str();
}

}       // namespace move

template<class CharT, class Traits, class Rules, class Board>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, action<Rules, Board> const& m)
{
        switch(move::getnotation<Rules, Board>(ostr)) {
        case Notation::algebraic : return move::print_algebraic(ostr, m);
        case Notation::numeric   : return move::print_numeric(ostr, m);
        default: assert(false); return ostr;
        }
}

}       // namespace dctl
