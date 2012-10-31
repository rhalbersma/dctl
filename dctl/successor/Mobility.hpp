#pragma once
#include <dctl/successor/Select.hpp>                 // Moves
#include <dctl/successor/enumeration/Enumerator.hpp> // enumerator
#include <dctl/node/Material.hpp>                    // both

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Position>
int mobility(Position const& p)
{
        return (detail::enumerator<Color, Material, Selection, Position>()(p));
}

template<bool Color, typename Position>
int mobility(Position const& p)
{
        return (mobility<Color, Material::both, Moves>(p));
}

}       // namespace successor
}       // namespace dctl
