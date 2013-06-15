#pragma once
#include <dctl/successor/count.hpp>             // count
#include <dctl/successor/select/moves.hpp>      // moves
#include <dctl/pieces/all.hpp>    // all

namespace dctl {
namespace successor {

template<bool Color, class Pieces, class Select, class Position>
int mobility(Position const& p)
{
        return count<Color, Pieces, Select>(p);
}

template<bool Color, class Position>
int mobility(Position const& p)
{
        return count<Color, pieces::all, select::moves>(p);
}

}       // namespace successor
}       // namespace dctl
