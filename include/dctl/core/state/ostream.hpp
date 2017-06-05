#pragma once
#include <dctl/core/state/setup/diagram.hpp>
#include <dctl/core/state/setup/string.hpp>
#include <dctl/core/state/state.hpp>
#include <dctl/core/state/ui/pdn/version.hpp>
#include <cassert>
#include <iosfwd>                       // ostream

namespace dctl {
namespace core {
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
        if (auto const iword = str.iword(setnotation()); iword) {
                return static_cast<stateformat>(iword);
        }
        return stateformat::diag;
}

}       // namespace manip

template<class Rules, class Board>
auto& operator<<(std::ostream& ostr, state<Rules, Board> const& s)
{
        switch (detail::getstateformat(ostr)) {
        case detail::stateformat::diag: return ostr << setup::diagram<pdn::protocol>()(s);
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

}       // namespace core
}       // namespace dctl
