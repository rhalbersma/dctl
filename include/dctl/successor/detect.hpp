#pragma once
#include <dctl/color.hpp>                               // Color, black, white
#include <dctl/successor/detect/specializations.hpp>    // Detect
#include <dctl/successor/select/legal.hpp>              // legal

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse = false, class Select = select::legal, class... Args, class Position>
auto detect(Position const& p)
{
        return Detect<ToMove, IsReverse, Select, Args...>{}(p);
}

template<bool IsReverse = false, class Select = select::legal, class... Args, class Position>
auto detect(Position const& p)
{
        return
                (p.to_move() == Color::black) ?
                detect<Color::black, IsReverse, Select, Args...>(p) :
                detect<Color::white, IsReverse, Select, Args...>(p)
        ;
}

}       // namespace successor
}       // namespace dctl
