#pragma once
#include <dctl/successor/Select.hpp>                 // DefaultSelection
#include <dctl/successor/enumeration/Enumerator.hpp> // enumerator
#include <dctl/node/Material.hpp>                    // both
#include <dctl/node/Side.hpp>                        // black, white

namespace dctl {
namespace successor {

template<int Material, typename Selection, typename Position>
int count(Position const& p)
{
        return (p.active_color() == Side::white)?
                detail::enumerator<Side::white, Material, Selection, Position>()(p) :
                detail::enumerator<Side::black, Material, Selection, Position>()(p)
        ;
}

template<typename Position>
int count(Position const& p)
{
        return count<Material::both, DefaultSelection>(p);
}

}       // namespace successor
}       // namespace dctl
