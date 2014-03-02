#pragma once
#include <dctl/successor/count.hpp>             // count
#include <dctl/successor/select/push.hpp>      // moves
#include <dctl/pieces/all.hpp>                  // all

namespace dctl {
namespace successor {

template<bool Color, class Pieces, class Select, class Position>
auto mobility(Position const& p)
{
        return count<Color, Pieces, Select>(p);
}

template<bool Color, class Position>
auto mobility(Position const& p)
{
        return count<Color, pieces::all, select::push>(p);
}

}       // namespace successor
}       // namespace dctl
