#pragma once
#include <dctl/successor/count.hpp>
#include <dctl/successor/detect.hpp>

namespace dctl {
namespace successor {

template<class Pieces, class Select, class Position>
auto invariant(Position const& p, int n)
{
        return (
                ( count<Pieces, Select>(p) ==  n     ) &&
                (detect<Pieces, Select>(p) == (n > 0))
        );
}

}       // namespace successor
}       // namespace dctl
