#pragma once
#include <dctl/successor/select.hpp>                 // Moves
#include <dctl/successor/enumeration/enumerator.hpp> // enumerator
#include <dctl/node/material.hpp>                    // both

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
