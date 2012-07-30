#pragma once
#include "Select.hpp"
#include "enumeration/Enumerator.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"

namespace dctl {
namespace successor {

template<typename Position>
int count(Position const& p)
{
        return count<Material::both, select::Legal>(p);
}

template<int Material, typename Selection, typename Position>
int count(Position const& p)
{
        return (p.active_color() == Side::white)?
                detail::enumerator<Side::white, Material, Selection, Position>()(p) :
                detail::enumerator<Side::black, Material, Selection, Position>()(p)
        ;
}

}       // namespace successor
}       // namespace dctl
