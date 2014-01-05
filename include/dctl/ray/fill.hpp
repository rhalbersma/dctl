#pragma once

namespace dctl {
namespace ray {

template<class Iterator, class Set>
auto fill(Iterator from, Set const& propagator)
{
        Set targets {};
        for (++from; propagator.test(*from); ++from)
                targets.set(*from);
        return targets;
}

}       // namespace ray
}       // namespace dctl
