#pragma once
#include <dctl/successor/select.hpp>    // Moves
#include <dctl/successor/count.hpp>     // count
#include <dctl/node/material.hpp>       // both

namespace dctl {
namespace successor {

template<bool Color, int Material, typename Selection, typename Position>
int mobility(Position const& p)
{
        return count<Color, Material, Selection>(p);
}

template<bool Color, typename Position>
int mobility(Position const& p)
{
        return count<Color, Material::both, Moves>(p);
}

}       // namespace successor
}       // namespace dctl
