#pragma once
#include <dctl/successor/select/legal.hpp>              // DefaultSelection
#include <dctl/successor/detect/specializations.hpp>    // detect
#include <dctl/node/side.hpp>                           // black, white
#include <dctl/pieces/pieces.hpp>                       // pawn, king, piece
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {

template<bool Color, class Pieces, class Select, class Position>
auto detect(Position const& p)
{
        using Rules = typename Position::rules_type;
        using Range = typename rules::range::king<Rules>::type;
        return detail::detect<Color, Pieces, Select, Range>{}(p);
}

template<class Pieces, class Select, class Position>
auto detect(Position const& p)
{
        return (p.to_move() == Side::black)?
                detect<Side::black, Pieces, Select>(p) :
                detect<Side::white, Pieces, Select>(p)
        ;
}

template<class Position>
auto detect(Position const& p)
{
        return detect<pieces::all, select::legal>(p);
}

}       // namespace successor
}       // namespace dctl
