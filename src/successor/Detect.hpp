#pragma once
#include "detection/Detector.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"

namespace dctl {
namespace successor {

template<typename Selection, typename Position>
bool detect(Position const& p)
{
        return detect<Material::both, Selection>(p);
}

template<int Material, typename Selection, typename Position>
bool detect(Position const& p)
{
        return (p.active_color() == Side::white)?
                detail::detector<Side::white, Material, Selection, Position>::run(p) :
                detail::detector<Side::black, Material, Selection, Position>::run(p)
        ;
}

}       // namespace successor
}       // namespace dctl
