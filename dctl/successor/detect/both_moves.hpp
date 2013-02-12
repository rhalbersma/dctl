#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/king_moves.hpp>    // detect (king moves specialization)
#include <dctl/successor/detect/impl/pawn_moves.hpp>    // detect (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/material.hpp>                       // Material
#include <dctl/rules/traits.hpp>                        // distance_1

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Range>
struct detect<Color, Material::both, select::moves, Range>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                typedef impl::detect<Color, Material::pawn, select::moves, Position, rules::range::distance_1> PiecesUpMoves;
                typedef impl::detect<Color, Material::king, select::moves, Position, rules::range::distance_1> KingDownMoves;

                // #pieces > #kings, so the logical OR is more likely to short-circuit
                Propagate<select::moves, Position> const propagate(p);
                return PiecesUpMoves{propagate}(p.pieces(Color)) || KingDownMoves{propagate}(p.kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
