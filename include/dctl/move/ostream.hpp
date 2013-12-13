#pragma once
#include <iosfwd>                       // ostream
#include <dctl/move/move.hpp>

namespace dctl {

template<class Rules, class Board>
decltype(auto) operator<<(std::ostream& ostr, Move<Rules, Board> const& m)
{
        ostr << m.notation();
        return ostr;
}

}       // namespace dctl
