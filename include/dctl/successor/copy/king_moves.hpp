#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/king_moves.hpp>      // copy (king moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/unary_projections.hpp>              // moveable_kings
#include <dctl/pieces/king.hpp>             // king

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves
template<bool Color>
struct copy<Color, pieces::king, select::moves>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                typedef impl::copy<Color, pieces::king, select::moves, Position, Sequence> KingMoves;

                Propagate<select::moves, Position> const propagate(p);
                KingMoves{propagate, moves}(moveable_kings(p, Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
