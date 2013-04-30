#pragma once
#include <cstddef>                                      // size_t
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/impl/pawn_moves.hpp>     // count (pawn moves specialization)
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/reverse.hpp>            // reverse
#include <dctl/successor/select/moves.hpp>              // moves

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for reverse pawn moves
template<bool Color>
struct count<Color, material::pawn, select::reverse>
{
        template<typename Position>
        std::size_t operator()(Position const& p) const
        {
                typedef impl::count<!Color, material::pawn, select::moves, Position> PawnReverse;

                Propagate<select::moves, Position> const propagate(p);
                return PawnReverse{propagate}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
