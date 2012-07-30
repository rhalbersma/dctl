#pragma once
#include "enumeration/Enumerator.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"

namespace dctl {
namespace successor {

template<typename Selection, typename Position>
int count(Position const& p)
{
        return count<Material::both, Selection>(p);
}

template<int Material, typename Selection, typename Position>
int count(Position const& p)
{
        return (p.active_color() == Side::white)?
                detail::enumerator<Side::white, Material, Selection, Position>::run(p) :
                detail::enumerator<Side::black, Material, Selection, Position>::run(p)
        ;
}

}       // namespace successor
}       // namespace dctl
