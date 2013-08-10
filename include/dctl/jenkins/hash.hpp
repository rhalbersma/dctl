#pragma once
#include <cstdint>                              // uint64_t
#include <dctl/jenkins/detail/random.hpp>       // Random
#include <dctl/node/side.hpp>                   // black, white

namespace dctl {
namespace jenkins {

// primary template
template<class Index = uint64_t, class Position>
Index hash(Position const& p)
{
        auto index = Index { 0 };

        detail::Random<Index>::mix(index, active_color(p));
        detail::Random<Index>::mix(index, p.material().pieces(Side::black));
        detail::Random<Index>::mix(index, p.material().pieces(Side::white));
        detail::Random<Index>::mix(index, p.material().kings());

        return index;
}

}       // namespace jenkins
}       // namespace dctl
