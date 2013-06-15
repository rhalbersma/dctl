#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/jumps.hpp>               // count (jumps specialization)
#include <dctl/successor/count/all_promotions.hpp>      // count (piece promotions specialization)
// there are no king promotions
#include <dctl/successor/count/pawn_promotions.hpp>     // count (pawn promotions specialization)
#include <dctl/successor/select/conversions.hpp>        // conversions
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/promotions.hpp>         // promotions

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for conversions
template<bool Color, class Pieces>
struct count<Color, Pieces, select::conversions>
{
        template<class Position>
        int operator()(Position const& p) const
        {
                typedef count<Color, Pieces, select::jumps     > DoJumps;
                typedef count<Color, Pieces, select::promotions> DoPromotions;

                auto num_moves = DoJumps()(p);
                if (!num_moves)
                        num_moves += DoPromotions()(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
