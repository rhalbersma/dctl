#pragma once
#include <cstddef>                                      // size_t
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/impl/pawn_moves.hpp>     // count (pawn moves specialization)
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/promotions.hpp>         // promotions
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/node/promotion.hpp>                      // promoting_pawns

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn promotions
template<bool Color>
struct count<Color, material::pawn, select::promotions>
{
        template<typename Position>
        std::size_t operator()(Position const& p) const
        {
                typedef impl::count<Color, material::pawn, select::moves, Position> PawnMoves;

                Propagate<select::moves, Position> const propagate(p);
                return PawnMoves{propagate}(promoting_pawns<Color>(p));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
