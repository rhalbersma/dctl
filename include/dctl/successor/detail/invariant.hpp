#pragma once
#include <dctl/color.hpp>               // Color
#include <dctl/successor/count.hpp>     // count
#include <dctl/successor/detect.hpp>    // detect

namespace dctl {
namespace successor {
namespace detail {

template<Color ToMove, bool IsReverse, class Select, class... Args, class Position>
auto invariant(Position const& p, std::size_t n)
{
        return
                ( count<ToMove, Select, IsReverse, Args...>(p) ==  n     ) &&
                (detect<ToMove, Select, IsReverse, Args...>(p) == (n > 0))
        ;
}

}       // namespace detail
}       // namespace successor
}       // namespace dctl
