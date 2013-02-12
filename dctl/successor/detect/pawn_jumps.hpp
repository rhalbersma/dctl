#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/impl/pawn_jumps.hpp>    // detect (pawn jumps specialization)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/node/material.hpp>                       // Material
#include <dctl/rules/traits.hpp>                        // distance_1

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps detection
template<bool Color, typename Range>
struct detect<Color, Material::pawn, select::jumps, Range>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                typedef impl::detect<Color, Material::pawn, select::jumps, Position, rules::range::distance_1> PawnJumps;

                Propagate<select::jumps, Position> const propagate(p);
                return PawnJumps{propagate}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
