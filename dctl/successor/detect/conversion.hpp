#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // detect (primary template)
#include <dctl/successor/detect/both_jumps.hpp>         // detect (combined king and pawn jumps specialization)
#include <dctl/successor/detect/both_promotions.hpp>    // detect (combined king and pawn promotions specialization)
#include <dctl/successor/detect/king_jumps.hpp>         // detect (king jumps specialization)
// there are no king promotions
#include <dctl/successor/detect/pawn_jumps.hpp>         // detect (pawn jumps specialization)
#include <dctl/successor/detect/pawn_promotions.hpp>    // detect (pawn promotions specialization)
#include <dctl/successor/select/conversion.hpp>         // conversion
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/successor/select/promotions.hpp>         // promotions
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for conversions
template<bool Color, int Material, typename Position, typename Range>
struct detect<Color, Material, select::conversion, Position, Range>
{
        bool operator()(Position const& p) const
        {
                typedef detect<Color, Material, select::jumps,      Position, Range                   > DoJumps;
                typedef detect<Color, Material, select::promotions, Position, rules::range::distance_1> DoPromotions;

                // speculate #jumps > #promotions, so that the || is likely to short-circuit
                return DoJumps()(p) || DoPromotions()(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
