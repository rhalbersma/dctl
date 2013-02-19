#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/piece_jumps.hpp>        // detect (piece jumps specialization)
#include <dctl/successor/detect/piece_promotions.hpp>   // detect (piece promotions specialization)
#include <dctl/successor/detect/king_jumps.hpp>         // detect (king jumps specialization)
// there are no king promotions
#include <dctl/successor/detect/pawn_jumps.hpp>         // detect (pawn jumps specialization)
#include <dctl/successor/detect/pawn_promotions.hpp>    // detect (pawn promotions specialization)
#include <dctl/successor/select/conversions.hpp>        // conversions
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/promotions.hpp>         // promotions
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for conversions
template<bool Color, typename Material, typename Range>
struct detect<Color, Material, select::conversions, Range>
{
        template<typename Position>
        bool operator()(Position const& p) const
        {
                typedef detect<Color, Material, select::jumps,      Range                   > DoJumps;
                typedef detect<Color, Material, select::promotions, rules::range::distance_1> DoPromotions;

                // speculate #jumps > #promotions, so that the || is likely to short-circuit
                return DoJumps()(p) || DoPromotions()(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl