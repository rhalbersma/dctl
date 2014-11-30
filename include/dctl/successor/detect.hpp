#pragma once
#include <dctl/color.hpp>                               // black, white
#include <dctl/successor/select/legal.hpp>              // DefaultSelection
#include <dctl/successor/detect/specializations.hpp>    // Detect
#include <dctl/pieces/pieces.hpp>                       // pawn, king, piece
#include <dctl/rule_traits.hpp>
#include <dctl/type_traits.hpp>

namespace dctl {
namespace successor {

template<Color ToMove, class Pieces, class Select, class Position>
auto detect(Position const& p)
{
        using Rules = rules_type_t<Position>;
        using Range = is_long_ranged_king_t<Rules>;
        return Detect<ToMove, Pieces, Select, Range>{}(p);
}

template<class Pieces, class Select, class Position>
auto detect(Position const& p)
{
        return
                (p.to_move() == Color::black)?
                detect<Color::black, Pieces, Select>(p) :
                detect<Color::white, Pieces, Select>(p)
        ;
}

template<class Position>
auto detect(Position const& p)
{
        return detect<pieces::all, select::legal>(p);
}

}       // namespace successor
}       // namespace dctl
