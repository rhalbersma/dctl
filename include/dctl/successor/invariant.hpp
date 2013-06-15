#pragma once
#include <dctl/successor/count.hpp>
#include <dctl/successor/detect.hpp>

namespace dctl {
namespace successor {

template<bool Color, class Pieces, class Select, class Position>
bool invariant(Position const& p, int n)
{
        return (
                ( count<Color, Pieces, Select>(p) ==  n     ) &&
                (detect<Color, Pieces, Select>(p) == (n > 0))
        );
}

}       // namespace successor
}       // namespace dctl
