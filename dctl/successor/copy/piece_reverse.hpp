#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/king_moves.hpp>      // copy (king moves specialization)
#include <dctl/successor/copy/impl/pawn_moves.hpp>      // copy (pawn moves specialization)
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/successor/material/king.hpp>             // king
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece reverse moves
template<bool Color>
struct copy<Color, material::piece, select::reverse>
{
        template<typename Position, typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef impl::copy<!Color, material::king, select::moves, Position, Vector> KingReverse;
                typedef impl::copy<!Color, material::pawn, select::moves, Position, Vector> PawnReverse;

                Propagate<select::moves, Position> const propagate(p);
                KingReverse{propagate, moves}(p.kings(Color));
                PawnReverse{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
