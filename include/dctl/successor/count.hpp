#pragma once
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/count/specializations.hpp>     // Count
#include <dctl/position/color.hpp>                      // black, white
#include <dctl/pieces/pieces.hpp>                       // piece, king, pawn

namespace dctl {
namespace successor {

template<bool Color, class Pieces, class Select, class Position>
auto count(Position const& p)
{
        return Count<Color, Pieces, Select>{}(p);
}

template<class Pieces, class Select, class Position>
auto count(Position const& p)
{
        return (p.active_color() == Color::black) ?
                count<Color::black, Pieces, Select>(p) :
                count<Color::white, Pieces, Select>(p)
        ;
}

template<class Position>
auto count(Position const& p)
{
        return count<pieces::all, select::legal>(p);
}

}       // namespace successor
}       // namespace dctl
