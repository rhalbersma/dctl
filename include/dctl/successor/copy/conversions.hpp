#pragma once
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/all_jumps.hpp>            // copy (all jumps specialization)
#include <dctl/successor/copy/all_promotions.hpp>       // copy (all promotions specialization)
#include <dctl/successor/copy/king_jumps.hpp>           // copy (king jumps specialization)
// there are no king promotions
#include <dctl/successor/copy/pawn_jumps.hpp>           // copy (pawn jumps specialization)
#include <dctl/successor/copy/pawn_promotions.hpp>      // copy (pawn promotions specialization)
#include <dctl/successor/select/conversions.hpp>        // conversions
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/promotions.hpp>         // promotions

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for conversions
template<bool Color, class Pieces>
struct copy<Color, Pieces, select::conversions>
{
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                typedef copy<Color, Pieces, select::jumps     > DoJumps;
                typedef copy<Color, Pieces, select::promotions> DoPromotions;

                DoJumps()(p, moves);
                if (moves.empty())
                        DoPromotions()(p, moves);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
