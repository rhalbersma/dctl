#pragma once
#include <dctl/color.hpp>               // Color
#include <dctl/successor/count.hpp>     // count
#include <dctl/successor/detect.hpp>    // detect
#include <cstddef>                      // size_t

namespace dctl {
namespace successor {
namespace detail {

template<Color ToMove, class Select, bool RemoveDuplicateJumps, bool Reverse, class Position>
auto invariant(Position const& p, std::size_t n)
{
        return
                ( count<ToMove, Select, RemoveDuplicateJumps, Reverse>(p) ==  n     ) &&
                (detect<ToMove, Select,                       Reverse>(p) == (n > 0))
        ;
}

}       // namespace detail
}       // namespace successor
}       // namespace dctl
