#pragma once
#include "enumeration/Enumerator.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Selection, typename Position>
int mobility(Position const& p)
{
        return mobility<Color, Material::both, Selection>(p);
}

template<bool Color, int Material, typename Selection, typename Position>
int mobility(Position const& p)
{
        return detail::enumerator<Color, Material, Selection, Position>::run(p);
}

}       // namespace successor
}       // namespace dctl
