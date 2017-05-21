#pragma once
#include <dctl/action/action.hpp>       // action
#include <dctl/action/manip.hpp>        // notation, pushsep, jumpsep
#include <dctl/action/traits.hpp>       // notation, pushsep, jumpsep
#include <dctl/rule_traits.hpp>         // jumpsep, pushsep
#include <dctl/utility/type_traits.hpp> // board_t, rules_t
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
        return pushsep_v<Rules>;
}

template<class Rules>
auto getjumpsep(std::ios_base& str)
{
        if (auto const iword = jumpsep(str))
                return iword;
        return jumpsep_v<Rules>;
}

template<class Action>
auto separator(std::ostream& ostr, Action const& a)
{
        using rules_type = rules_t<Action>;
        return (a.is_jump() ? getjumpsep<rules_type>(ostr) : getpushsep<rules_type>(ostr));
}

template<class Action>
auto& print_algebraic(std::ostream& ostr, Action const& m)
{
        using board_type = board_t<Action>;
        ostr << board_type::algebraic_from_bit(m.from());
        ostr << separator(ostr, m);
        ostr << board_type::algebraic_from_bit(m.dest());
        return ostr;
}

template<class Action>
auto& print_numeric(std::ostream& ostr, Action const& a)
{
        using board_type = board_t<Action>;
        ostr << board_type::numeric_from_bit(a.from());
        ostr << separator(ostr, a);
        ostr << board_type::numeric_from_bit(a.dest());
        return ostr;
}

template<class Action>
auto str_algebraic(Action const& a)
{
        std::stringstream sstr;
        print_algebraic(sstr, a);
        return sstr.str();
}

template<class Action>
auto str_numeric(Action const& a)
{
        std::stringstream sstr;
        print_numeric(sstr, a);
        return sstr.str();
}

}       // namespace move

template<class CharT, class Traits, class Rules, class Board>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, action<Rules, Board> const& a)
{
        switch(move::getnotation<Rules, Board>(ostr)) {
        case Notation::algebraic : return move::print_algebraic(ostr, a);
        case Notation::numeric   : return move::print_numeric(ostr, a);
	default: assert(false); return ostr;
        }
}

}       // namespace dctl
