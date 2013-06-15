#pragma once
#include <dctl/successor/select/legal.hpp>              // DefaultSelection
#include <dctl/successor/detect/specializations.hpp>    // detect
#include <dctl/node/side.hpp>                           // black, white
#include <dctl/pieces/pieces.hpp>                       // pawn, king, piece
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {

template<bool Color, class Pieces, class Select, class Position>
bool detect(Position const& p)
{
        typedef typename Position::rules_type Rules;
        typedef typename rules::range::king<Rules>::type Range;
        return detail::detect<Color, Pieces, Select, Range>()(p);
}

template<class Pieces, class Select, class Position>
bool detect(Position const& p)
{
        return (p.to_move() == Side::black)?
                detect<Side::black, Pieces, Select>(p) :
                detect<Side::white, Pieces, Select>(p)
        ;
}

template<class Position>
bool detect(Position const& p)
{
        return detect<pieces::all, select::legal>(p);
}

}       // namespace successor
}       // namespace dctl
