#pragma once
#include <dctl/color.hpp>                               // Color, black, white
#include <dctl/successor/count/specializations.hpp>     // Count
#include <dctl/successor/select/legal.hpp>              // legal

namespace dctl {
namespace successor {

template<Color ToMove, class Select = select::legal, bool IsReverse = false, class... Args, class Position>
auto count(Position const& p)
{
        return Count<ToMove, Select, IsReverse, Args...>{}(p);
}

template<class Select = select::legal, bool IsReverse = false, class... Args, class Position>
auto count(Position const& p)
{
        return
                (p.to_move() == Color::black) ?
                count<Color::black, Select, IsReverse, Args...>(p) :
                count<Color::white, Select, IsReverse, Args...>(p)
        ;
}

}       // namespace successor
}       // namespace dctl
