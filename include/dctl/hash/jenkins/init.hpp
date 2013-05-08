#pragma once
#include <functional>                   // function
#include <dctl/hash/jenkins/random.hpp>
#include <dctl/node/side.hpp>

namespace dctl {
namespace hash {
namespace jenkins {

// primary template
template<typename Index, typename Position>
struct Init
{
        Index operator()(Position const& p) const
        {
                Index index = 0;

                Random<Index>::mix(index, active_color(p));
                Random<Index>::mix(index, p.material().pieces(Side::black));
                Random<Index>::mix(index, p.material().pieces(Side::white));
                Random<Index>::mix(index, p.material().kings());

                return index;
        }
};

}       // namespace jenkins
}       // namespace hash
}       // namespace dctl
