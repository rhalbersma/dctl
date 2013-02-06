#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/impl/king_moves.hpp>     // count (king moves specialization)
#include <dctl/successor/count/impl/pawn_moves.hpp>     // count (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/material.hpp>                       // Material

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for combined king and pawn reverse moves
template<bool Color>
struct count<Color, Material::both, select::reverse>
{
        template<typename Position>
        int operator()(Position const& p) const
        {
                typedef impl::count<!Color, Material::king, select::moves, Position> KingReverse;
                typedef impl::count<!Color, Material::pawn, select::moves, Position> PawnReverse;

                Propagate<select::moves, Position> const propagate(p);
                return KingReverse{propagate}(p.kings(Color)) + PawnReverse{propagate}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
