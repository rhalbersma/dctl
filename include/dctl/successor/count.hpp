#pragma once
#include <dctl/color.hpp>                               // Color, black, white
#include <dctl/successor/count/specializations.hpp>     // Count
#include <dctl/successor/select/legal.hpp>              // legal

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse = false, class Select = select::legal, class... Args, class Position>
auto count(Position const& p)
{
        return Count<ToMove, IsReverse, Select, Args...>{}(p);
}

template<bool IsReverse = false, class Select = select::legal, class... Args, class Position>
auto count(Position const& p)
{
        return
                (p.to_move() == Color::black) ?
                count<Color::black, IsReverse, Select, Args...>(p) :
                count<Color::white, IsReverse, Select, Args...>(p)
        ;
}

}       // namespace successor
}       // namespace dctl
