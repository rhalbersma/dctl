#pragma once
#include <dctl/color.hpp>                               // Color, black, white
#include <dctl/successor/count/specializations.hpp>     // Count
#include <dctl/successor/select/legal.hpp>              // legal

namespace dctl {
namespace successor {

template<Color ToMove, class Select = select::legal, bool RemoveDuplicateJumps = true, bool Reverse = false, class Position>
auto count(Position const& p)
{
        return Count<ToMove, Select, RemoveDuplicateJumps, Reverse>{}(p);
}

template<class Select = select::legal, bool RemoveDuplicateJumps = true, bool Reverse = false, class Position>
auto count(Position const& p)
{
        return
                (p.to_move() == Color::black) ?
                count<Color::black, Select, RemoveDuplicateJumps, Reverse>(p) :
                count<Color::white, Select, RemoveDuplicateJumps, Reverse>(p)
        ;
}

}       // namespace successor
}       // namespace dctl
