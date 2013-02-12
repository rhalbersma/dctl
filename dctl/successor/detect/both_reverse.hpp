#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/king_moves.hpp>    // detect (king moves specialization)
#include <dctl/successor/detect/impl/pawn_moves.hpp>    // detect (pawn moves specialization)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/material.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Range>
struct detect<Color, Material::both, select::reverse, Range>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                typedef impl::detect<!Color, Material::king, select::moves, Position, rules::range::distance_1> KingReverse;
                typedef impl::detect<!Color, Material::pawn, select::moves, Position, rules::range::distance_1> PawnReverse;

                // speculate #pawns > #kings so that the || is likely to short-circuit
                Propagate<select::moves, Position> const propagate(p);
                return PawnReverse{propagate}(p.kings(Color)) || KingReverse{propagate}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
