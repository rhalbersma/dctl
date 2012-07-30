#pragma once
#include "Select.hpp"
#include "generation/Generator.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"
#include "../node/Stack.hpp"

namespace dctl {
namespace successor {

template<typename Position>
void generate(Position const& p, Stack& moves)
{
        generate<Material::both, select::Legal>(p, moves);
}

template<int Material, typename Selection, typename Position>
void generate(Position const& p, Stack& moves)
{
        if (p.active_color() == Side::white)
                detail::generator<Side::white, Material, Selection, Position>()(p, moves);
        else
                detail::generator<Side::black, Material, Selection, Position>()(p, moves);      
}

}       // namespace successor
}       // namespace dctl
