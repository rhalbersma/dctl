#pragma once
#include <dctl/successor/count.hpp>                     // count
#include <dctl/successor/material/piece.hpp>            // both
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {

template<bool Color, typename Material, typename Select, typename Position>
int mobility(Position const& p)
{
        return count<Color, Material, Select>(p);
}

template<bool Color, typename Position>
int mobility(Position const& p)
{
        return count<Color, material::piece, select::moves>(p);
}

}       // namespace successor
}       // namespace dctl
