#pragma once
#include "Driver.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"
#include "../node/Stack.hpp"

namespace dctl {
namespace successor {

template<typename Selection, typename Position>
void generate(Position const& p, Stack& moves)
{
        generate<Material::both, Selection>(p, moves);
}

template<int Material, typename Selection, typename Position>
void generate(Position const& p, Stack& moves)
{
        if (p.active_color() == Side::white)
                generator<Side::white, Material, Selection, Position>::run(p, moves);
        else
                generator<Side::black, Material, Selection, Position>::run(p, moves);      
}

template<typename Selection, typename Position>
int count(Position const& p)
{
        return count<Material::both, Selection>(p);
}

template<int Material, typename Selection, typename Position>
int count(Position const& p)
{
        return (p.active_color() == Side::white)?
                counter<Side::white, Selection, Position>::run(p) :
                counter<Side::black, Selection, Position>::run(p)
        ;
}

template<typename Selection, typename Position>
bool detect(Position const& p)
{
        return detect<Material::both, Selection>(p);
}

template<int Material, typename Selection, typename Position>
bool detect(Position const& p)
{
        return (p.active_color() == Side::white)?
                detector<Side::white, Selection>::run(p) :
                detector<Side::black, Selection>::run(p)
        ;
}

}       // namespace successor
}       // namespace dctl
