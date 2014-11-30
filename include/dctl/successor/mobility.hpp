#pragma once
#include <dctl/color.hpp>
#include <dctl/successor/count.hpp>             // count
#include <dctl/successor/select/push.hpp>      // moves
#include <dctl/pieces/all.hpp>                  // all

namespace dctl {
namespace successor {

template<Color ToMove, class Pieces, class Select, class Position>
auto mobility(Position const& p)
{
        return count<ToMove, Pieces, Select>(p);
}

template<Color ToMove, class Position>
auto mobility(Position const& p)
{
        return count<ToMove, pieces::all, select::push>(p);
}

}       // namespace successor
}       // namespace dctl
