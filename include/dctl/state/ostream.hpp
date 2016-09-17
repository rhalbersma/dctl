#pragma once
#include <dctl/ui/pdn/version.hpp>
#include <dctl/state/state.hpp>
#include <dctl/state/manip/position_format.hpp>
#include <dctl/setup/diagram.hpp>
#include <dctl/setup/string.hpp>
#include <cassert>
#include <iosfwd>                       // ostream

namespace dctl {

inline
auto getpositionformat(std::ostream& ostr)
{
        auto index = ostr.iword(manip::position_format());
        if (!index)
                index = static_cast<int>(manip::StateFormat::diag);
        return static_cast<manip::StateFormat>(index);
}

template<class CharT, class Traits, class Rules, class Board>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, state<Rules, Board> const& p)
{
        switch (getpositionformat(ostr)) {
        case manip::StateFormat::diag : return ostr << setup::diagram<pdn::protocol>()(p);
        case manip::StateFormat::fen  : return ostr << setup::write<pdn::protocol>()(p);
        }
}

}       // namespace dctl
