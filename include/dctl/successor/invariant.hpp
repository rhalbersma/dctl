#pragma once
#include <dctl/successor/count.hpp>
#include <dctl/successor/detect.hpp>

namespace dctl {
namespace successor {

template<bool Color, typename Material, typename Select, typename Position>
bool invariant(Position const& p, int n)
{
        return (
                ( count<Color, Material, Select>(p) ==  n     ) &&
                (detect<Color, Material, Select>(p) == (n > 0))
        );
}

}       // namespace successor
}       // namespace dctl
