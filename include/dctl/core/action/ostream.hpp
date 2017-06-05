#pragma once
#include <dctl/core/action/action.hpp>  // action
#include <dctl/core/action/manip.hpp>   // notation, pushsep, jumpsep
#include <dctl/core/rules/traits.hpp>   // rules_t, pushsep, jumpsep, notation
#include <dctl/util/type_traits.hpp>    // board_t, rules_t
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <cassert>                      // assert
#include <iosfwd>                       // ostream
#include <sstream>                      // stringstream

namespace dctl {
namespace core {
namespace detail {

template<class Action>
auto separator(std::ios_base& str, Action const& a)
{
        using rules_type = rules_t<Action>;
        if (a.is_jump()) {
                if (auto const iword = getjumpsep(str); iword) {
                        return iword;
                }
                return jumpsep_v<rules_type>;
        } else {
                if (auto const iword = getpushsep(str); iword) {
                        return iword;
                }
                return pushsep_v<rules_type>;
        }
}

template<class Action>
auto& print_algebraic(std::ostream& ostr, Action const& a)
{
        using board_type = board_t<Action>;
        ostr << board_type::algebraic_from_bit(a.from());
        ostr << separator(ostr, a);
        ostr << board_type::algebraic_from_bit(a.dest());
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

}       // namespace detail

template<class Rules, class Board>
auto& operator<<(std::ostream& ostr, action<Rules, Board> const& a)
{
        switch([&ostr]() {
                if (auto const iword = getnotation(ostr); xstd::to_underlying_type(iword)) {
                        return iword;
                }
                return notation_v<Rules>;
        }()) {
        case notation::algebraic : return detail::print_algebraic(ostr, a);
        case notation::numeric   : return detail::print_numeric(ostr, a);
        }
	return ostr;
}

template<class Action>
auto str_algebraic(Action const& a)
{
        std::stringstream sstr;
        detail::print_algebraic(sstr, a);
        return sstr.str();
}

template<class Action>
auto str_numeric(Action const& a)
{
        std::stringstream sstr;
        detail::print_numeric(sstr, a);
        return sstr.str();
}

}       // namespace core
}       // namespace dctl
