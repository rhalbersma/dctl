#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/king_moves.hpp>      // copy (king moves specialization)
#include <dctl/successor/copy/impl/pawn_moves.hpp>      // copy (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/pieces/pieces.hpp>                  // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece reverse moves
template<bool Color>
struct copy<Color, pieces::all, select::reverse>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                typedef impl::copy<!Color, pieces::king, select::moves, Position, Sequence> KingReverse;
                typedef impl::copy<!Color, pieces::pawn, select::moves, Position, Sequence> PawnReverse;

                Propagate<select::moves, Position> const propagate(p);
                KingReverse{propagate, moves}(p.material().kings(Color));
                PawnReverse{propagate, moves}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
