#pragma once
#include <dctl/core/action/action.hpp>          // action
#include <dctl/core/rules/type_traits.hpp>      // pushsep_v, jumpsep_v, notation_v
#include <dctl/util/type_traits.hpp>            // board_t, rules_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <cassert>                              // assert
#include <ios>                                  // ios_base
#include <ostream>                              // basic_ostream
#include <sstream>                              // stringstream

namespace dctl::core {

class setpushsep
{
        char m_iword;
public:
        explicit setpushsep(char const c) noexcept
        :
                m_iword{c}
        {}

        auto iword() const noexcept
        {
                return m_iword;
        }

        static auto index()
        {
                static auto const i = std::ios_base::xalloc();
                return i;
        }
};

inline
auto& operator<<(std::ios_base& str, setpushsep const& manip)
{
        str.iword(manip.index()) = manip.iword();
        return str;
}

class setjumpsep
{
        char m_iword;
public:
        explicit setjumpsep(char const c) noexcept
        :
                m_iword{c}
        {}

        auto iword() const noexcept
        {
                return m_iword;
        }

        static auto index()
        {
                static auto const i = std::ios_base::xalloc();
                return i;
        }
};

inline
auto& operator<<(std::ios_base& str, setjumpsep const& manip)
{
        str.iword(manip.index()) = manip.iword();
        return str;
}

namespace detail {

template<class Action>
auto separator(std::ios_base& str, Action const& a)
{
        using rules_type = rules_t<Action>;
        if (a.is_jump()) {
                if (auto const iword = str.iword(setpushsep::index()); iword) {
                        return static_cast<char>(iword);
                }
                return jumpsep_v<rules_type>;
        } else {
                if (auto const iword = str.iword(setpushsep::index()); iword) {
                        return static_cast<char>(iword);
                }
                return pushsep_v<rules_type>;
        }
}

inline
auto setnotation()
{
        static auto const i = std::ios_base::xalloc();
        return i;
}

template<class Action>
auto getnotation(std::ios_base& str, Action const&)
{
        using rules_type = rules_t<Action>;
        if (auto const iword = str.iword(setnotation()); iword) {
                return static_cast<notation>(iword);
        }
        return notation_v<rules_type>;
}

template<class CharT, class Traits, class Action>
auto& print_algebraic(std::basic_ostream<CharT, Traits>& ostr, Action const& a)
{
        using board_type = board_t<Action>;
        ostr << board_type::algebraic_from_bit(a.from());
        ostr << separator(ostr, a);
        ostr << board_type::algebraic_from_bit(a.dest());
        return ostr;
}

template<class CharT, class Traits, class Action>
auto& print_numeric(std::basic_ostream<CharT, Traits>& ostr, Action const& a)
{
        using board_type = board_t<Action>;
        ostr << board_type::numeric_from_bit(a.from());
        ostr << separator(ostr, a);
        ostr << board_type::numeric_from_bit(a.dest());
        return ostr;
}

}       // namespace detail

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

template<class CharT, class Traits, class Rules, class Board>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, action<Rules, Board> const& a)
{
        switch(detail::getnotation(ostr, a)) {
        case notation::algebraic: return detail::print_algebraic(ostr, a);
        case notation::numeric  : return detail::print_numeric(ostr, a);
        }
        return ostr;
}

inline
auto& algebraic(std::ios_base& str)
{
        str.iword(detail::setnotation()) = xstd::to_underlying_type(notation::algebraic);
        return str;
}

inline
auto& numeric(std::ios_base& str)
{
        str.iword(detail::setnotation()) = xstd::to_underlying_type(notation::numeric);
        return str;
}

}       // namespace dctl::core
