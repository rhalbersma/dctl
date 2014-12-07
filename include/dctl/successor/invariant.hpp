#pragma once
#include <dctl/color.hpp>               // Color
#include <dctl/successor/count.hpp>     // count
#include <dctl/successor/detect.hpp>    // detect

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse, class Select, class... Args, class Position>
auto invariant(Position const& p, std::size_t n)
{
        return
                ( count<ToMove, IsReverse, Select, Args...>(p) ==  n     ) &&
                (detect<ToMove, IsReverse, Select, Args...>(p) == (n > 0))
        ;
}

}       // namespace successor
}       // namespace dctl
