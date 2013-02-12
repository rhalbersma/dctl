#pragma once
#include <dctl/successor/select/legal.hpp>            // DefaultSelection
#include <dctl/successor/detect/specializations.hpp>   // detect
#include <dctl/node/material.hpp>               // both
#include <dctl/node/side.hpp>                   // black, white

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Position>
bool detect(Position const& p)
{
        typedef typename Position::rules_type Rules;
        typedef typename rules::traits<Rules>::king_range Range;
        return detail::detect<Color, Material, Selection, Range>()(p);
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
        return detect<Material::both, select::legal>(p);
}

}       // namespace successor
}       // namespace dctl
