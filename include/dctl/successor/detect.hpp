#pragma once
#include <dctl/color.hpp>                               // Color, black, white
#include <dctl/successor/detect/specializations.hpp>    // Detect
#include <dctl/successor/select/legal.hpp>              // legal

namespace dctl {
namespace successor {

template<Color ToMove, class Select = select::legal, bool Reverse = false, class Position>
auto detect(Position const& p)
{
        return Detect<ToMove, Select, Reverse>{}(p);
}

template<class Select = select::legal, bool Reverse = false, class Position>
auto detect(Position const& p)
{
        return
                (p.to_move() == Color::black) ?
                detect<Color::black, Select, Reverse>(p) :
                detect<Color::white, Select, Reverse>(p)
        ;
}

}       // namespace successor
}       // namespace dctl
