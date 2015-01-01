#pragma once
#include <dctl/color.hpp>                               // Color, black, white
#include <dctl/successor/detect/specializations.hpp>    // Detect
#include <dctl/successor/select/legal.hpp>              // legal

namespace dctl {
namespace successor {

template<Color ToMove, class Select = select::legal, bool IsReverse = false, class Position>
auto detect(Position const& p)
{
        return Detect<ToMove, Select, IsReverse>{}(p);
}

template<class Select = select::legal, bool IsReverse = false, class Position>
auto detect(Position const& p)
{
        return
                (p.to_move() == Color::black) ?
                detect<Color::black, Select, IsReverse>(p) :
                detect<Color::white, Select, IsReverse>(p)
        ;
}

}       // namespace successor
}       // namespace dctl
