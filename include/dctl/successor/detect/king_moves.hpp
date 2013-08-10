#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/king_moves.hpp>    // detect (king moves specialization)
#include <dctl/pieces/king.hpp>             // king
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/unary_projections.hpp>              // moveable_kings
#include <dctl/rules/traits.hpp>                        // distance_1

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves detection
template<bool Color, class Range>
struct detect<Color, pieces::king, select::moves, Range>
{
        template<class Position>
        bool operator()(Position const& p) const
        {
                using KingMoves = impl::detect<Color, pieces::king, select::moves, Position, rules::range::distance_1>;

                Propagate<select::moves, Position> const propagate(p);
                return KingMoves{propagate}(moveable_kings(p, Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
