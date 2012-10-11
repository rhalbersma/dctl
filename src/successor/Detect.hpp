#pragma once
#include "Select.hpp"
#include "detection/Detector.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"

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
