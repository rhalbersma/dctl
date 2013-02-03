#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/aux/king_moves.hpp>       // copy (king moves specialization)
#include <dctl/successor/copy/aux/pawn_moves.hpp>       // copy (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/node/material.hpp>                       // Material

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for combined king and pawn reverse moves
template<bool Color>
struct copy<Color, Material::both, select::reverse>
{
        template<typename Position, typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef aux::copy<!Color, Material::king, select::moves, Position> KingReverse;
                typedef aux::copy<!Color, Material::pawn, select::moves, Position> PawnReverse;

                Propagate<select::moves, Position> const propagate(p);
                KingReverse{propagate, moves}(p.kings(Color));
                PawnReverse{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
