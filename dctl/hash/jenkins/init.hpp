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
:
        std::function<Index(Position const&)>
{
        Index operator()(Position const& p) const
        {
                Index index = 0;

                Random<Index>::mix(index, p.active_color());
                Random<Index>::mix(index, p.pieces(Side::black));
                Random<Index>::mix(index, p.pieces(Side::white));
                Random<Index>::mix(index, p.kings());

                return (index);
        }
};

}       // namespace jenkins
}       // namespace hash
}       // namespace dctl
