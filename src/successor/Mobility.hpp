#pragma once
#include "Select.hpp"
#include "enumeration/Enumerator.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Position>
int mobility(Position const& p)
{
        return mobility<Color, Material::both, Moves>(p);
}

template<bool Color, int Material, typename Selection, typename Position>
int mobility(Position const& p)
{
        return detail::enumerator<Color, Material, Selection, Position>()(p);
}

}       // namespace successor
}       // namespace dctl
