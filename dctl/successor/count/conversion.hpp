#pragma once
#include <dctl/successor/count/primary_fwd.hpp>         // count (primary template)
#include <dctl/successor/count/jumps.hpp>               // count (jumps specialization)
#include <dctl/successor/count/both_promotions.hpp>     // count (combined king and pawn promotions specialization)
// there are no king promotions
#include <dctl/successor/count/pawn_promotions.hpp>     // count (pawn promotions specialization)
#include <dctl/successor/select/conversion.hpp>         // conversions
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/promotions.hpp>         // promotions

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for conversions
template<bool Color, int Material>
struct count<Color, Material, select::conversion>
{
        template<typename Position>
        int operator()(Position const& p) const
        {
                typedef count<Color, Material, select::jumps     > DoJumps;
                typedef count<Color, Material, select::promotions> DoPromotions;

                auto num_moves = DoJumps()(p);
                if (!num_moves)
                        num_moves += DoPromotions()(p);
                return num_moves;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
