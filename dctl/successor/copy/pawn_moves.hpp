#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/aux/pawn_moves.hpp>       // copy (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/material.hpp>                       // Material

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves
template<bool Color>
struct copy<Color, Material::pawn, select::moves>
{
        template<typename Position, typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef aux::copy<Color, Material::pawn, select::moves, Position> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                PawnMoves{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
