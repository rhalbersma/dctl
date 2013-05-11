#pragma once
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/count/specializations.hpp>     // count
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/node/side.hpp>                           // black, white

namespace dctl {
namespace successor {

template<bool Color, typename Material, typename Select, typename Position>
int count(Position const& p)
{
        return detail::count<Color, Material, Select>()(p);
}

template<typename Material, typename Select, typename Position>
int count(Position const& p)
{
        return (p.to_move() == Side::black)?
                count<Side::black, Material, Select>(p) :
                count<Side::white, Material, Select>(p)
        ;
}

template<typename Position>
int count(Position const& p)
{
        return count<material::piece, select::legal>(p);
}

}       // namespace successor
}       // namespace dctl
