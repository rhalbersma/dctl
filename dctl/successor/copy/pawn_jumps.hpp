#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/pawn_jumps.hpp>      // copy (pawn jumps specialization)
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps
template<bool Color>
struct copy<Color, material::pawn, select::jumps>
{
        template<typename Position, typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef impl::copy<Color, material::pawn, select::jumps, Position, Vector> PawnJumps;

                Propagate<select::jumps, Position> propagate(p);
                PawnJumps{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
