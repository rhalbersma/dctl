#pragma once
#include <dctl/core/state/ui/pdn/version.hpp>
#include <dctl/core/state/state.hpp>
#include <dctl/core/state/manip/position_format.hpp>
#include <dctl/core/state/setup/diagram.hpp>
#include <dctl/core/state/setup/string.hpp>
#include <cassert>
#include <iosfwd>                       // ostream

namespace dctl {
namespace core {

inline
auto getpositionformat(std::ostream& ostr)
{
        auto index = ostr.iword(manip::position_format());
        if (!index)
                index = static_cast<int>(manip::StateFormat::diag);
        return static_cast<manip::StateFormat>(index);
}

template<class CharT, class Traits, class Rules, class Board>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, state<Rules, Board> const& s)
{
        switch (getpositionformat(ostr)) {
        case manip::StateFormat::diag : return ostr << setup::diagram<pdn::protocol>()(s);
        case manip::StateFormat::fen  : return ostr << setup::write<pdn::protocol>()(s);
	default: assert(false); return ostr;
        }
}

}       // namespace core
}       // namespace dctl
