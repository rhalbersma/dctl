#pragma once
#include <dctl/successor/count.hpp>
#include <dctl/successor/detect.hpp>

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Position>
bool invariant(Position const& p,  int N)
{
        return (
                ( count<Color, Material, Selection>(p) ==  N     ) &&
                (detect<Color, Material, Selection>(p) == (N > 0))
        );
}

}       // namespace successor
}       // namespace dctl
