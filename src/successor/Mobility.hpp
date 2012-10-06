#pragma once
#include "Select.hpp"
#include "enumeration/Enumerator.hpp"
#include "../node/Material.hpp"

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Position>
int mobility(Position const& p)
{
        return detail::enumerator<Color, Material, Selection, Position>()(p);
}

template<bool Color, typename Position>
int mobility(Position const& p)
{
        return mobility<Color, Material::both, Moves>(p);
}

}       // namespace successor
}       // namespace dctl
