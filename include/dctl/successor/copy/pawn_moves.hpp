#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/pawn_moves.hpp>      // copy (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/pieces/pawn.hpp>             // pawn

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves
template<bool Color>
struct copy<Color, pieces::pawn, select::moves>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using PawnMoves = impl::copy<Color, pieces::pawn, select::moves, Position, Sequence>;

                Propagate<select::moves, Position> const propagate(p);
                PawnMoves{propagate, moves}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
