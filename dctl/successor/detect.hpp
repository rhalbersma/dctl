#pragma once
#include <dctl/successor/select.hpp>            // DefaultSelection
#include <dctl/successor/detect/detector.hpp>   // detector
#include <dctl/node/material.hpp>               // both
#include <dctl/node/side.hpp>                   // black, white

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Position>
bool detect(Position const& p)
{
        return detail::detector<Color, Material, Selection, Position>()(p);
}

template<int Material, typename Selection, typename Position>
bool detect(Position const& p)
{
        return (p.active_color() == Side::black)?
                detect<Side::black, Material, Selection>(p) :
                detect<Side::white, Material, Selection>(p)
        ;
}

template<typename Position>
bool detect(Position const& p)
{
        return detect<Material::both, DefaultSelection>(p);
}

}       // namespace successor
}       // namespace dctl
