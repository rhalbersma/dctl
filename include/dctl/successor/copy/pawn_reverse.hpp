#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/pawn_moves.hpp>      // copy (pawn moves specialization)
#include <dctl/pieces/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for reverse pawn moves
template<bool Color>
struct copy<Color, pieces::pawn, select::reverse>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                typedef impl::copy<!Color, pieces::pawn, select::moves, Position, Sequence> PawnReverse;

                Propagate<select::moves, Position> const propagate(p);
                PawnReverse{propagate, moves}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
