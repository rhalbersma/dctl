#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/pawn_jumps.hpp>      // copy (pawn jumps specialization)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/pieces/pawn.hpp>                         // pawn

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps
template<bool Color>
struct copy<Color, pieces::pawn, select::jumps>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using PawnJumps = impl::copy<Color, pieces::pawn, select::jumps, Position, Sequence>;

                Propagate<select::jumps, Position> propagate(p);
                PawnJumps{propagate, moves}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
