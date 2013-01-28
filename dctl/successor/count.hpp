#pragma once
#include <dctl/successor/select.hpp>            // DefaultSelection
#include <dctl/successor/count/enumerator.hpp>  // enumerator
#include <dctl/node/material.hpp>               // both
#include <dctl/node/side.hpp>                   // black, white

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Position>
int count(Position const& p)
{
        return detail::enumerator<Color, Material, Selection, Position>()(p);
}

template<int Material, typename Selection, typename Position>
int count(Position const& p)
{
        return (p.active_color() == Side::black)?
                count<Side::black, Material, Selection>(p) :
                count<Side::white, Material, Selection>(p)
        ;
}

template<typename Position>
int count(Position const& p)
{
        return count<Material::both, DefaultSelection>(p);
}

}       // namespace successor
}       // namespace dctl
