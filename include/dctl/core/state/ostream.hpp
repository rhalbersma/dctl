#pragma once
#include <dctl/core/board/diagram.hpp>
#include <dctl/core/state/detail/content.hpp>
#include <dctl/core/state/setup/string.hpp>
#include <dctl/core/state/state.hpp>
#include <dctl/core/state/ui/pdn.hpp>
#include <cassert>
#include <ios>                                  // ios_base
#include <ostream>                              // basic_ostream

namespace dctl::core {
namespace detail {

enum class stateformat
{
        /* zero is reserved */
        diag = 1,
        fen = 2
};

inline
auto setstateformat()
{
        static auto const i = std::ios_base::xalloc();
        return i;
}

inline
auto getstateformat(std::ios_base& str)
{
        if (auto const iword = str.iword(setstateformat()); iword) {
                return static_cast<stateformat>(iword);
        }
        return stateformat::diag;
}

}       // namespace detail

template<class CharT, class Traits, class Rules, class Board>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, state<Rules, Board> const& s)
{
        switch (detail::getstateformat(ostr)) {
        case detail::stateformat::diag: return ostr << diagram<Board>{}([&](auto const n) { return detail::content<detail::token_set<pdn::protocol>>(s, n); });
        case detail::stateformat::fen : return ostr << setup::write<pdn::protocol>()(s);
        }
        return ostr;
}

inline
auto& diag(std::ios_base& str)
{
        str.iword(detail::setstateformat()) = xstd::to_underlying_type(detail::stateformat::diag);
        return str;
}

inline
auto& fen(std::ios_base& str)
{
        str.iword(detail::setstateformat()) = xstd::to_underlying_type(detail::stateformat::fen);
        return str;
}

}       // namespace dctl::core
