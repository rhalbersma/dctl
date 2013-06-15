#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/pawn_moves.hpp>
#include <dctl/pieces/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/promotions.hpp>         // promotions
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/promotion.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, class Range>
struct detect<Color, pieces::pawn, select::promotions, Range>
{
        template<class Position>
        bool operator()(Position const& p) const
        {
                typedef impl::detect<Color, pieces::pawn, select::moves, Position, rules::range::distance_1> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                return PawnMoves{propagate}(promoting_pawns<Color>(p));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
