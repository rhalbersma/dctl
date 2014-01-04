#pragma once
#include <iterator>                             // next

namespace dctl {
namespace ray {

template<class Iterator, class Set>
auto fill(Iterator src, Set const& propagator)
{
        Set dst {};
        for (auto it = std::next(src); propagator.test(*it); ++it)
                dst.set(*it);
        return dst;
}

}       // namespace ray
}       // namespace dctl
