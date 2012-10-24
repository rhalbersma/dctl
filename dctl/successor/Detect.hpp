#pragma once
#include <dctl/successor/Select.hpp>                 // DefaultSelection
#include <dctl/successor/detection/Detector.hpp>     // detector
#include <dctl/node/Material.hpp>                    // both
#include <dctl/node/Side.hpp>                        // black, white

namespace dctl {
namespace successor {

template<int Material, typename Selection, typename Position>
bool detect(Position const& p)
{
        return (p.active_color() == Side::white)?
                detail::detector<Side::white, Material, Selection, Position>()(p) :
                detail::detector<Side::black, Material, Selection, Position>()(p)
        ;
}

template<typename Position>
bool detect(Position const& p)
{
        return detect<Material::both, DefaultSelection>(p);
}

}       // namespace successor
}       // namespace dctl
