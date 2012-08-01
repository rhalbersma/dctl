#pragma once
#include "Count.hpp"
#include "Detect.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<int Material, typename Selection, typename Position>
bool invariant(Position const& p,  int number)
{
        return (
                ( count<Material, Selection>(p) ==  number     ) &&
                (detect<Material, Selection>(p) == (number > 0))
        );
}

}       // namespace detail
}       // namespace successor
}       // namespace dctl
