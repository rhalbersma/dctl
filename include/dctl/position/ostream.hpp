#pragma once
#include <dctl/pdn/version.hpp>
#include <dctl/position/position.hpp>
#include <dctl/position/manip/position_format.hpp>
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
                index = static_cast<int>(manip::PositionFormat::diag);
        return static_cast<manip::PositionFormat>(index);
}

template<class CharT, class Traits, class Rules, class Board>
auto& operator<<(std::basic_ostream<CharT, Traits>& ostr, Position<Rules, Board> const& p)
{
        switch (getpositionformat(ostr)) {
        case manip::PositionFormat::diag : return ostr << setup::diagram<pdn::protocol>()(p);
        case manip::PositionFormat::fen  : return ostr << setup::write<pdn::protocol>()(p);
        }
}

}       // namespace dctl
