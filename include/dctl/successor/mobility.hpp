#pragma once
#include <dctl/color.hpp>                       // Color
#include <dctl/successor/count.hpp>             // count
#include <dctl/successor/select/push.hpp>       // push

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse = false, class Select = select::push, class... Args, class Position>
auto mobility(Position const& p)
{
        return count<ToMove, IsReverse, Select, Args...>(p);
}

}       // namespace successor
}       // namespace dctl
