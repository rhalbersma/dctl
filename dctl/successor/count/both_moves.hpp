#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/impl/king_moves.hpp>     // count (king moves specialization)
#include <dctl/successor/count/impl/pawn_moves.hpp>     // count (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/material.hpp>                       // Material

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for combined king and pawn moves
template<bool Color>
struct count<Color, Material::both, select::moves>
{
        template<typename Position>
        int operator()(Position const& p) const
        {
                typedef impl::count<Color, Material::king, select::moves, Position> KingMoves;
                typedef impl::count<Color, Material::pawn, select::moves, Position> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                return KingMoves{propagate}(p.kings(Color)) + PawnMoves{propagate}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
