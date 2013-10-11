#pragma once
#include <dctl/successor/select/legal.hpp>              // legal
#include <dctl/successor/count/specializations.hpp>     // count
#include <dctl/node/side.hpp>                           // black, white
#include <dctl/pieces/pieces.hpp>                // piece, king, pawn

namespace dctl {
namespace successor {

template<bool Color, class Pieces, class Select, class Position>
auto count(Position const& p)
{
        return detail::count<Color, Pieces, Select>()(p);
}

template<class Pieces, class Select, class Position>
auto count(Position const& p)
{
        return (p.to_move() == Side::black) ?
                count<Side::black, Pieces, Select>(p) :
                count<Side::white, Pieces, Select>(p)
        ;
}

template<class Position>
auto count(Position const& p)
{
        return count<pieces::all, select::legal>(p);
}

}       // namespace successor
}       // namespace dctl
