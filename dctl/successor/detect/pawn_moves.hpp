#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/pawn_moves.hpp>    // detect (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/material.hpp>                       // Material
#include <dctl/rules/traits.hpp>                        // distance_1

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves detection
template<bool Color, typename Range>
struct detect<Color, Material::pawn, select::moves, Range>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                typedef impl::detect<Color, Material::pawn, select::moves, Position, rules::range::distance_1> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                return PawnMoves{propagate}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
