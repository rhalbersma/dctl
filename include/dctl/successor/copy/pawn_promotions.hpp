#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/pawn_moves.hpp>      // copy (pawn moves specialization)
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/promotions.hpp>         // promotions
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/promotion.hpp>                      // promoting_pawns

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn promotions
template<bool Color>
struct copy<Color, material::pawn, select::promotions>
{
        template<typename Position, typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef impl::copy<Color, material::pawn, select::moves, Position, Vector> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                PawnMoves{propagate, moves}(promoting_pawns<Color>(p));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
